#include "ModelLoading/modelloadingelenode.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <limits>

ModelLoadingEleNode::ModelLoadingEleNode():ModelLoadingStrategy(){
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","ele"));
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","node"));
	}

ModelLoadingEleNode::~ModelLoadingEleNode(){}

/***
* @brief Verifies that a given file can be loaded by this class
*
* For this file format, we must check that both .ele and .node files are placed
* in the same place and have the same name.
*
* @param filename	Path of an .ele or a .node file of a model
* @return boolean
*/
bool ModelLoadingEleNode::validate(std::string filename){
	std::string filenameExt = FileUtils::getFileNameExtension(filename);
	if(filenameExt.compare("ele") == 0 || filenameExt.compare("node") == 0){
		std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
		if(FileUtils::fileExists(filenameNoExt+".ele")&&FileUtils::fileExists(filenameNoExt+".node"))
			return true;
		}
	return false;
	}

/***
* @brief Transfers data from both .ele and .node files to a Model instance
*
* Camaron supports two specifications for this ASCII format, which depend on
* the program used to generate the files, in the case of TetGen .ele files
* contain a list of tetrahedrons while files generated using TRIANGLE use .ele
* files to store triangles.
*
* @param filename Path of a .ele or .node file of a 3D model
* @returns Model* Model containing the loaded data
*
* @throws std::bad_alloc  If the file cannot be saved inside the RAM
* @throws ModelLoadingException		If a file reaches the EOF unexpectedly
*
* @see ELE/Node format specifications:
* @see TetGen	   http://wias-berlin.de/software/tetgen/1.5/doc/manual/manual006.html
* @see TRIANGLE	   https://www.cs.cmu.edu/~quake/triangle.html
*/
Model* ModelLoadingEleNode::load(std::string filename){
	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);
	path = filenameNoExt;
	parser.setCurrentEncoding(Encoding::ASCII);
	Model* model = new Model(filename);

	try{
		readHeaderEle(model);
		readHeaderNode(model);
		if(model->is2D()){
			// TRIANGLE model
			emit setupProgressBarForNewModel(numberOfNodes, numberOfElements,0);
			readVertices(model);
			readPolygons(model);
			}
		else{
			// TetGen Model
			emit setupProgressBarForNewModel(numberOfNodes, 0, numberOfElements);
			readVertices(model);
			readPolyhedrons(model);
			}
		}
	catch(std::bad_alloc& e){
		if(model)
			delete model;
		parser.closeFile();
		indexVsPosition.clear();
		throw e;
		}
	catch(ModelLoadingException& e){
		if(model)
			delete model;
		parser.closeFile();
		indexVsPosition.clear();
		throw e;
		}

	parser.closeFile();
	return model;
	}

/***
* @brief Obtain number of elements composing the 3D model
*
* For TetGen generated files, the .ele file will contain a list of tetrahedrons
* with 4 or 10 nodes, in contrast a TRIANGLE model will list 3 or 6 nodes per
* triangle listed. Due to this we can determine the type of file based on the
* number of nodes listed in the header (located in the first line of the file).
*
* @param  model*	Pointer to the active Model instance
*
* @throws ModelLoadingException
*/
void ModelLoadingEleNode::readHeaderEle(Model* model){
	parser.openFile(path+".ele");
	parser >> numberOfElements >> numberOfNodesPerElement >> numberOfAttributesPerElement;
	switch(numberOfNodesPerElement){
		case 3:
		case 6: {
			model->reservePolygons(numberOfNodesPerElement);
			break;
			}
		case 4:
		case 10: {
			model->reservePolyhedrons(numberOfNodesPerElement);
			break;
			}
		default:
			throw ModelLoadingException(path, "Unrecognized model type");
		}
	parser.closeFile();
}

/***
* @brief Obtain number of nodes composing the 3D model
*
* TetGen models use three dimensional vertices while TRIANGLE uses two, as its
* use case revolves in the generation of 2D surfaces. Other properties as attributes
* and boundary markers can be obtained from the first line of the file.
*
* @param  model*		Pointer to the active Model instance
*/
void ModelLoadingEleNode::readHeaderNode(Model* model){
	parser.openFile(path+".node");
	parser >> numberOfNodes >> dimensions >> numberOfAttributesPerNode >> numberOfBoundaryMarkers;
	parser.closeFile();

	model->set2D(dimensions==2);
	model->reserveVertices(numberOfNodes);
	}


/***
* @brief Read a sequence of vertices in ASCII format from a .node file
*
* Vertices are described using a sequence of numbers per row, where row distribution
* will depend on the specification obtained from the header.
*
* As vertices are read, the boundary box of the model is updated if a new
* vertex is located outside of it.
*
* @param  mesh*		Pointer to	Model instance that will store the vertices
* @throws ModelLoadingError
*/
void ModelLoadingEleNode::readVertices(Model* mesh){
	parser.openFile(path+".node");
	parser.prepareNextLine();  //Skip header

	std::vector<vis::Vertex>& vertices = mesh->getVertices();
	std::vector<float> &bounds = mesh->getBounds();
	ElementsRelations* relations = mesh->getElementsRelations();

	int index;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	for(int i = 0; i< numberOfNodes; i++){
		parser >> index >> x >> y;
		if(dimensions>2)
			parser >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		vertices.emplace_back(index, x, y, z);
		updateBoundingBox(bounds, x, y, z);

		relations->addIdAndPositionInContainerVertex(index, i);

		if(i%5000==0)
			emit setLoadedVertices(i);

		parser.prepareNextLine();
		}

	relations->checkVertices();

	parser.closeFile();
	emit setLoadedVertices(numberOfNodes);
}

/***
* @brief Read a sequence of polygons in ASCII format
*
* Method used exclusively by TRIANGLE generated meshes, each row represents
* a triangle, listing its index and the ids of its vertices. This methods ignore
* auxiliary vertices that could be generated by the -o2 option in TRIANGLE.
*
* @param mesh*	   Pointer to Model instance that will store each polygon
* @throws ModelLoadingException
*/
void ModelLoadingEleNode::readPolygons(Model* mesh){
	parser.openFile(path+".ele");
	parser.prepareNextLine();

	std::vector<vis::Polygon>& triangles = mesh->getPolygons();

	mesh->getElementsRelations()->getVertexPolygons().reserve(triangles.capacity());

	for(int i = 0; i < numberOfElements; i++){
		int index, v1, v2, v3;
		parser >> index >> v1 >> v2 >> v3;
		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested polygons");
			}
		triangles.emplace_back(index);
		
		mesh->getElementsRelations()->getVertexPolygons().emplace_back();

		mesh->getElementsRelations()->addVertexInPolygon(index, v1);
		mesh->getElementsRelations()->addVertexInPolygon(index, v2);
		mesh->getElementsRelations()->addVertexInPolygon(index, v3);
		
		parser.prepareNextLine();
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(numberOfElements);

	indexVsPosition.clear();
	parser.closeFile();
	}

/***
* @brief Read a sequence of polyhedrons in ASCII format
*
* Method used exclusively by TetGen generated objects, each row represents a
* tetrahedron, listing its index and its 4 respective vertices. This method
* will ignore auxiliary vertices that could be generated by the -o2 option in TetGen
*
* Due to the fact that polygon information is not included in this specification,
* we must create Polygon instances to represent the Polyhedrons correctly in the model,
* to do this we employ the usage of a Hashing Tree, which will keep track of all the
* triangles composing each tetrahedron, this ensures that we are not allocating
* repeated polygons into the heap.
*
* @param mesh*	   Pointer to Model instance that will store the Polyhedrons
* @throws ModelLoadingException
*/
void ModelLoadingEleNode::readPolyhedrons(Model* mesh){
	parser.openFile(path+".ele");
	parser.prepareNextLine();
	std::vector<vis::Polyhedron>& tetrahedrons = mesh->getPolyhedrons();

	vis::ut::HashingTree<int,int> hashingTree;

	int v[4];
	int tetrahedronIndex = 0;
	int polygonCount = 0;

	/// Vertex order for each face according to specs (Prevents inverted normals)
	int vertOrder[4][3] = {{0, 2, 1}, {0, 1, 2}, {0, 2, 1}, {0, 1, 2}};

	mesh->reservePolygons(4 * numberOfElements);
	mesh->getElementsRelations()->getPolygonsPolyhedrons().reserve(numberOfElements);
	mesh->getElementsRelations()->getPolyhedronPolygons().reserve(4 * numberOfElements);

	for (int i = 0 ; i < numberOfElements; i++){
		parser >> tetrahedronIndex >> v[0] >> v[1] >> v[2] >> v[3];
		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested polyhedrons");
			}

		tetrahedrons.emplace_back(tetrahedronIndex);
		for(int j = 0; j < 4; j++){
			std::vector<int> triangleVertices = {v[j], v[(j+1)%4], v[(j+2)%4]};
			int polygonIndex = hashingTree.getRegistry(triangleVertices);

			// if that polygon (triangle) does not exist, it'll create
			if(polygonIndex == -1){
				mesh->getPolygons().emplace_back(polygonCount);
				mesh->getElementsRelations()->getVertexPolygons().emplace_back();
				mesh->getElementsRelations()->getPolyhedronPolygons().emplace_back();
				mesh->getElementsRelations()->getPolygonsPolygons().emplace_back();

				// Add vector polygon relation
				for (unsigned int k = 0; k < triangleVertices.size(); k++){
					mesh->getElementsRelations()->addVertexInPolygon(polygonCount, triangleVertices[vertOrder[j][k]]);
				}
				
				// Add a relation between a polyedron polygon
				hashingTree.registerValue(triangleVertices, polygonCount);
				mesh->getElementsRelations()->addPolyhedronInPolygon(tetrahedronIndex, polygonCount);
				mesh->getElementsRelations()->addPolygonInPolyhedron(tetrahedronIndex, polygonCount);
				polygonCount++;

			}
			else{
				mesh->getElementsRelations()->addPolyhedronInPolygon(tetrahedronIndex, polygonIndex);
				mesh->getElementsRelations()->addPolygonInPolyhedron(tetrahedronIndex, polygonIndex);
			}
		}
		parser.prepareNextLine();
		if(i%5000==0){
			emit setLoadedPolyhedrons(i);
		}
	}
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingEleNode);
