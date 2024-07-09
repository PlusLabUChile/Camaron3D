#include "modelloadingvisf.h"
#include "Model/Model.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Utils/endianess.h"
#include "ModelLoading/MeshProcessor.h"

ModelLoadingVisF::ModelLoadingVisF():
	ModelLoadingStrategy("VisF","visf"){}

ModelLoadingVisF::~ModelLoadingVisF(){
	parser.closeFile();
	}


/***
* @note The VisF model already stores polygon-polygon vecinity information, thus
* we can skip the call for completePolygonPolygonRelations method, located inside
* the inherited implementation of completeMesh ModelStrategy. This requires
* overriding the inherited version with this one instead.
*/
void ModelLoadingVisF::processPolygons(Model* mesh){
	MeshProcessor::completeVertexPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	MeshProcessor::calculateNormalsPolygons(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	MeshProcessor::calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
	}

void ModelLoadingVisF::processPolyhedrons(Model* mesh) {
	MeshProcessor::completeVertexPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	MeshProcessor::completePolygonPolyhedronRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
	MeshProcessor::calculateGeoCenterPolyhedronMesh(mesh);
	emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
	MeshProcessor::calculateNormalsPolygons(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
	MeshProcessor::calculateNormalsVertices(mesh);
	}

///

/***
* @brief Verifies that a given file can be loaded by this class
*
* For this file format, the current implementation checks that the first byte
* has a value of 0 (big endian), 1 (little endian) or 2 ((ASCII) and that the
* following 4 bytes (int) have a value between 0 and 2 (mesh type).
*
* @note The following implementation attempts to open for both v0.2 and v0.3
*		 specifications, deprecate v0.2 and remove it afterwards
*
* @param filename  Path of a .visf file containing the model
* @return boolean
*/
bool ModelLoadingVisF::validate(std::string filename){
	unsigned char fileEncoding;
	unsigned char currentSystemEndianness = Endianess::findEndianness();
	// First attempt to read assuming an ASCII header (v0.3)
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	parser >> fileEncoding;
	switch (fileEncoding){
		case 0:
		case 1: {
			if (fileEncoding != currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::EXTERNAL_BINARY);
				}
			else {
				parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
				}
			break;
			}
		case 2: {
			parser.setCurrentEncoding(Encoding::ASCII);
			break;
			}
		default: {
			/// @deprecated If it doesn't work, attempt to read the header as
			/// a binary value (v0.2). Delete this entire block afterwards
			parser.closeFile();
			parser.openFile(filename);
			parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
			parser >> fileEncoding;

			if (fileEncoding > 1){
				return false;
				}

			if (fileEncoding != currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::EXTERNAL_BINARY);
				}
			else if (fileEncoding == currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
				}
			}
			///
		}
	int type;
	parser >> type;
	parser.closeFile();

	if (type >= 0 && type <=2){
		return true;
		}

	return false;
}

/***
* @brief Transfers data from a .visf file to a Model instance
*
* The .VisF extension is a custom file format , which encodes data from vertices,
* polygons and polyhedrons from a mesh, as well as storing the neighborhood
* relationship between polygons. Data begins after reading the first two numbers
* at the beginning of the file.
*
* @param filename  Path to file to be loaded
* @returns Model* Pointer to the active Model instance
*
* @see VisF format specification (Page 80) http://repositorio.uchile.cl/handle/2250/113645
*/
Model* ModelLoadingVisF::load(std::string filename){
	parser.openFile(filename);
	parser.setCurrentEncoding(Encoding::ASCII);
	path = filename;

	Model* model = new Model(filename);
	readHeader();
	try {
		readVertices(model);
		readPolygons(model);
		readPolyhedrons(model);
		}
	catch(std::bad_alloc &ba){
		if(model)
			delete model;
		parser.closeFile();
		throw std::bad_alloc();
		}
	catch(ModelLoadingException &ex){
		if(model)
			delete model;
		parser.closeFile();
		throw ex;
		}

	parser.closeFile();
	return model;
	}

/***
* @brief Reads initial header values
*
* Similar in nature to validate, with the difference that the model type and
* file encoding data is stored instead of discarded.
*
* @note The following implementation attempts to open for both v0.2 and v0.3
*		 specifications, deprecate v0.2 and remove it afterwards
* @note Refactor to reduce code duplication
*
* @param filename  Path of a .visf file containing the model
* @throws ModelLoadingException
*/
void ModelLoadingVisF::readHeader(){
	unsigned char currentSystemEndianness = Endianess::findEndianness();
	unsigned char fileEncoding;

	parser >> fileEncoding;
	switch (fileEncoding){
		case 0:
		case 1: {
			if (fileEncoding != currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::EXTERNAL_BINARY);
				}
			else {
				parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
				}
			break;
			}
		case 2: {
			parser.setCurrentEncoding(Encoding::ASCII);
			break;
			}
		default: {
			/// @deprecated If it doesn't work, attempt to read the header as
			/// a pure binary format (v0.2). Delete this entire block after the
			/// specification is well defined
			parser.closeFile();
			parser.openFile(path);
			parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
			parser >> fileEncoding;

			if (fileEncoding > 1){
				throw ModelLoadingException(path, "Unrecognized model type");
				}

			if (fileEncoding != currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::EXTERNAL_BINARY);
				}
			else if (fileEncoding == currentSystemEndianness){
				parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
				}
			}
			///
		}

	int modelType;
	parser >> modelType;
	parser.prepareNextLine();
	}


/***
* @brief Read a sequence of vertices in binary format from a .visf file
*
* Vertices are written as a sequence of three float values per vertex for its
* position. At the beginning of the sequence an int value is used to store
* the number of vertices contained in the file.
*
* As vertices are read, the boundary box of the model is updated if a new
* vertex is located outside of it.
*
* @param mesh*		Pointer to Model instance that will store the model vertices
* @throw ModelLoadingException
*/
void ModelLoadingVisF::readVertices(Model* mesh){
	std::vector<float> &bounds = mesh->getBounds();
	int nVertices;

	parser >> nVertices;
	parser.prepareNextLine();

	mesh->reserveVertices(nVertices);
	emit setupProgressBarForNewModel(nVertices,0,0);
	std::vector<vis::Vertex>& vertices = mesh->getVertices();

	float x, y, z;
	for(int i = 0; i < nVertices; i++){
		parser >> x >> y >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		vertices.emplace_back(i,x,y,z);
		updateBoundingBox(bounds, x, y, z);

		parser.prepareNextLine();
		if(i%1000==0)
			emit setLoadedVertices(i);
		}

	emit setLoadedVertices(nVertices);
	}

/***
*  @brief Read a sequence of polygons in Binary format
*
*  The polygon block start with an integer counting the number of polygons stored
*  in the file, afterwards for each polygon in the block, the first integer will
*  contain the number of vertices that compose the polygon, followed by the ids
*  of said vertices.
*
*  After reading the polygons, the next data block stores the polygon neighborhood
*  of each polygon, the sequence starts with the number of neighbor polygons followed
*  by the ids of said polygons, repeating the sequence for all remaining polygons.
*
*  @param mesh*		Pointer to the Model that will store the loaded polygons
*  @throws ModelLoadingException
*/
void ModelLoadingVisF::readPolygons(Model* mesh){
	int nPolygons;

	parser >> nPolygons;
	if (parser.invalidState() || !nPolygons){
		return;
		}

	parser.prepareNextLine();

	mesh->reservePolygons(nPolygons);
	std::vector<vis::Polygon>& polygons = mesh->getPolygons();
	std::vector<vis::Vertex>& vertices = mesh->getVertices();
	ElementsRelations* relations = mesh->getElementsRelations();
	
	emit setupProgressBarForNewModel(mesh->getVerticesCount(),nPolygons,0);
	emit setLoadedVertices(mesh->getVerticesCount());

	relations->getVertexPolygons().reserve(nPolygons);

	for(int i = 0;i < nPolygons; i++){
		int nVertices;
		parser >> nVertices;

		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested polygons");
			}

		polygons.emplace_back(i);

		relations->getPolygonsPolygons().emplace_back();
		relations->getVertexPolygons().emplace_back();

		for(int j = 0;j<nVertices;j++){
			int vertexId;
			parser >> vertexId;
			relations->addVertexInPolygon(i, vertexId);	
		}

		// getEdgesAndPolygons(mesh, relations->getVertexPolygonsById(i), i);


		parser.prepareNextLine();
		if(i%5000==0)
			emit setLoadedPolygons(i);
	}
	emit setLoadedPolygons(nPolygons);
	int hasNeighbors;
	parser >> hasNeighbors;
	parser.prepareNextLine();

	relations->getPolygonsPolygons().reserve(nPolygons);

	if(hasNeighbors){
		for(int i = 0;i<nPolygons;i++){
			int nNeighbors;
			parser >> nNeighbors;

			if (parser.invalidState()){
				throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF while reading neighborhood data");
			}
		
			relations->getPolygonsPolygons().emplace_back();

			for(int j = 0; j < nNeighbors; j++){
				int neighborId;
				parser >> neighborId;
				relations->addNeighborPolygonInPolygon(i, neighborId);
			}
		}
		parser.prepareNextLine();
	}
	else{
		MeshProcessor::completePolygonPolygonRelations(mesh);
	}
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
}

/***
* @brief Read a sequence of polyhedrons in Binary format
*
* Polyhedrons are stored in a similar fashion as with the polygons of the model,
* with the difference that there is no neighborhood block associated with each
* polyhedrons in the model.
*
* @param mesh*	   Pointer to Model instance that will store the Polyhedrons
* @throws ModelLoadingException
*/
void ModelLoadingVisF::readPolyhedrons(Model* mesh){
	int nPolyhedrons;
	parser >> nPolyhedrons;

	if (parser.invalidState() || !nPolyhedrons){
		return;
		}

	parser.prepareNextLine();
	mesh->reservePolyhedrons(nPolyhedrons);

	std::vector<vis::Polyhedron>& polyhedrons = mesh->getPolyhedrons();
	ElementsRelations* relations = mesh->getElementsRelations();

	emit setupProgressBarForNewModel(mesh->getVerticesCount(),
									 mesh->getPolygonsCount(),nPolyhedrons);
	emit setLoadedVertices(mesh->getVerticesCount());
	emit setLoadedPolygons(mesh->getPolygonsCount());
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);

	relations->getPolygonsPolyhedrons().reserve(nPolyhedrons);

	for(int i = 0; i < nPolyhedrons; i++){
		int nPolygons;
		parser >> nPolygons;

		if (parser.invalidState()){
			throw ModelLoadingException(mesh->getFilename(), "ERROR: Reached EOF before reading all requested polyhedrons");
			}

		polyhedrons.emplace_back(i);
		relations->getPolygonsPolyhedrons().emplace_back();

		for(int j = 0; j < nPolygons; j++){
			int polygonId;
			parser >> polygonId;
			relations->addPolygonInPolyhedron(i, polygonId);
		}

		parser.prepareNextLine();
		if(i%5000==0){
			emit setLoadedPolyhedrons(i);
		}
	}
	emit setLoadedPolyhedrons(nPolyhedrons);
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingVisF);
