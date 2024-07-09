#include "modelloadingm3d.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include <iostream>
#include <string.h>
ModelLoadingM3D::ModelLoadingM3D():ModelLoadingStrategy("M3D", "m3d"){}

ModelLoadingM3D::~ModelLoadingM3D(){
	parser.closeFile();
	}


bool ModelLoadingM3D::validate(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	std::string word;

	parser >> word;
	if (word.compare("[Nodes,")){
		parser.closeFile();
		return false;
		}

	parser >> word;
	parser.closeFile();
	return !word.compare("ARRAY1<STRING>]");
	}

Model* ModelLoadingM3D::load(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	Model* loaded = new Model(filename);
	try{
		readVertices(loaded);
		readPolyhedrons(loaded);
		parser.closeFile();
		return loaded;
		}

	catch(std::bad_alloc &ba){
		if(loaded)
			delete loaded;
		parser.closeFile();
		throw std::bad_alloc();
		}

	catch(ModelLoadingException &ex){
		delete loaded;
		parser.closeFile();
		throw ex;
		}
	}

void ModelLoadingM3D::readVertices(Model* model) {
	int nvertices = 0;
	parser.prepareNextLine();
	parser >> nvertices;

	model->reserveVertices(nvertices);
	std::vector<vis::Vertex> &v = model->getVertices();
	std::vector<float> &bounds = model->getBounds();

	emit setupProgressBarForNewModel(nvertices,0,0);

	int discard;
	float x, y, z;

	for(int i = 0; i < nvertices; i++){
		parser >> discard >> x >> y >> z;
		if(discard == 2){
			float x2, y2, z2;
			parser >> x2 >> y2 >> z2;
			x+=x2;y+=y2;z+=z2;
			}

		v.emplace_back(i, x, y, z);
		updateBoundingBox(bounds, x , y, z);

		if(i%5000==0){
			emit setLoadedVertices(i);
			}
		parser.prepareNextLine();
		}

	emit setLoadedVertices(nvertices);
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingM3D] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
}

bool ModelLoadingM3D::readPolyhedrons(Model* model) {
	std::cout << "ReadPolyhedrons\n";
	if(!moveToElements(model))
		return false;

	unsigned int polyhedronCount= model->getPolyhedrons().capacity();
	std::vector<vis::Polygon> &polygons = model->getPolygons();
	emit setupProgressBarForNewModel(model->getVerticesCount(),
									 polyhedronCount,
									 polyhedronCount);
	emit setLoadedVertices(model->getVerticesCount());
	vis::ut::HashingTree<int,int> hashingTree;

	//Polygon creation buffers
	unsigned int polygonCount = 0;
	std::vector<int> vertexNumbers;
	std::vector<int> vertexComponents;
	std::vector<std::array<int, 4>> polyhedronRel;

	std::string word;
	for(unsigned int i = 0; i < polyhedronCount; i++ ) {
		parser >> word;
		switch(word[0]){
			case 'T':{
				readM3DPolyhedron(4, tetrahedronIndices,
							      model, hashingTree,
								  polygonCount);
				break;
				}
			case 'P':{
				readM3DPolyhedron(5, pyramidIndices,
								  model, hashingTree,
								  polygonCount);
				break;
				}
			case 'R':{
				readM3DPolyhedron(6, prismIndices,
								  model, hashingTree,
								  polygonCount);
				break;
				}
			case 'H':{
				readM3DPolyhedron(8, hexahedronIndices,
								  model, hashingTree,
								  polygonCount);
				break;
				}
			default:{
				break;
				}
			}

		if(i%5000==0){
			emit setLoadedPolyhedrons(i);
			}
		parser.prepareNextLine();
		}

	std::cout << "PolygonVector: Capacity = " << polygons.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << polygons.size() << std::endl;
	return true;
	}

/*
void ModelLoadingM3D::readM3DPolyhedron(int nVertices,
										std::vector<std::vector<int> >& verticesIndicesPerFace,
										Model* model,
										vis::ut::HashingTree<int,int>& hashingTree,
										std::vector<int>& vertexNumbers,
										std::vector<int>& vertexComponents,
										std::vector<std::array<int, 4>>& polyhedronRel){

	std::vector<vis::Polyhedron>& polyhedrons = model->getPolyhedrons();
	std::vector<int> verticesIndices;
	verticesIndices.resize(nVertices);
	for(int v = 0; v < nVertices; v++){
		parser >> verticesIndices[v];
		}

	polyhedrons.emplace_back(polyhedrons.size());
	vis::Polyhedron* polyhedron = &polyhedrons[polyhedrons.size()-1];

	for(std::vector<std::vector<int>>::size_type i = 0; i < verticesIndicesPerFace.size(); i++){
		std::vector<int> aux;
		for(std::vector<int>::size_type j = 0; j < verticesIndicesPerFace[i].size(); j++){
			aux.push_back(verticesIndices[verticesIndicesPerFace[i][j]]);
			}

		int polygonIndex = hashingTree.getRegistry(aux);
		if(polygonIndex == -1){
			vertexNumbers.push_back(aux.size());
			for (unsigned int k = 0; k < aux.size(); k++){
				vertexComponents.push_back(aux[k]);
				}
			hashingTree.registerValue(aux, (int)vertexNumbers.size()-1);
			polyhedronRel.push_back(std::array<int, 4>{(int)polyhedrons.size()-1, (int)i, -1, -1});
			}
		else{
			polyhedronRel[polygonIndex][2] = polyhedrons.size()-1;
			polyhedronRel[polygonIndex][3] = i;
			}
		}
	}
*/

void ModelLoadingM3D::readM3DPolyhedron(int nVertices,
										std::vector<std::vector<int>>& verticesIndicesPerFace,
										Model* model,
										vis::ut::HashingTree<int,int>& hashingTree,
										unsigned int& polygonCount){
	std::cout << "readM3DPolyhedron\n";
	std::vector<vis::Polyhedron>& polyhedrons = model->getPolyhedrons();
	ElementsRelations* relations = relations;
	std::vector<int> verticesIndices;
	verticesIndices.resize(nVertices);
	for(int v = 0; v < nVertices; v++){
		parser >> verticesIndices[v];
	}

	polyhedrons.emplace_back(polyhedrons.size());
	relations->getPolygonsPolyhedrons().emplace_back();

	for(std::vector<std::vector<int>>::size_type i = 0; i < verticesIndicesPerFace.size(); i++){
		std::vector<int> aux;
		for(std::vector<int>::size_type j = 0; j < verticesIndicesPerFace[i].size(); j++){
			aux.push_back(verticesIndices[verticesIndicesPerFace[i][j]]);
		}

		int polygonIndex = hashingTree.getRegistry(aux);
		if(polygonIndex == -1){
			// Create polygon
			model->getPolygons().emplace_back(polygonCount);
			relations->getVertexPolygons().emplace_back();
			relations->getPolygonsPolyhedrons().emplace_back();
			relations->getPolygonsPolygons().emplace_back();

			for (unsigned int k = 0; k < aux.size(); k++){
				relations->getVertexPolygonsById(polygonCount).push_back(aux[k]);
			}

			relations->getPolygonsPolygons().emplace_back();
			// getEdgesAndPolygons(model, relations->getVertexPolygonsById(polygonCount), polygonCount);

			hashingTree.registerValue(aux, polygonCount);
			relations->addPolyhedronInPolygon(polyhedrons.size()-1, polygonCount);
			relations->addPolygonInPolyhedron(polyhedrons.size()-1, polygonCount);			
			polygonCount++;
		}
		
		else{
			// Polygon exist
			relations->addPolyhedronInPolygon(polyhedrons.size()-1, polygonIndex);
			relations->addPolygonInPolyhedron(polyhedrons.size()-1, polygonIndex);			
		}
	}
}

bool ModelLoadingM3D::moveToElements(Model* model){
	std::string word;

	while(true){
		parser >> word;
		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached end of file before locating polyhedron block ");
			}

		if (!word.compare("[Elements,")){
			parser >> word;
			if (!word.compare("ARRAY1<STRING>]")){
				int nElements;
				parser >> nElements;
				model->reservePolyhedrons(nElements);
				return true;
				}
			return false;
			}
		}
	}

#include "Factories/ModelLoadingFactory.h"
// This fuction gives support to .m3d files, but at the present this file format isn't used
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingM3D);

