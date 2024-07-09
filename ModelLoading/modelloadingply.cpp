#include "ModelLoading/modelloadingply.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Edge.h"
#include "Model/Model.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include <iostream>
#include <limits>
#include <string.h>

ModelLoadingPly::ModelLoadingPly():ModelLoadingStrategy("PLY", "ply"){}

ModelLoadingPly::~ModelLoadingPly(){
	parser.closeFile();
	}

/***
* @brief Verifies that a given file can be loaded by this class
*
* For the PLY format, we must verify that the file contains an ASCII header
* containing the lines "ply" at the start and "end_header" at the end.
*
* @todo Include the end_header check
*
* @param filename  Path containing the file to be loaded
* @return boolean
*/
bool ModelLoadingPly::validate(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	std::string word;

	parser >> word;
	parser.closeFile();
	return !word.compare("ply");
}

/***
* @brief Transfers data from a file to a Model instance
*
* Data from this format can be encoded using ASCII or binary format
* (big or little endian). Generating a Vertex Cloud or a Polygon Mesh based
* on its contents and including any additional edges that are included
* in the file.
*
* In the case of a polygon mesh, the method also computes shared metrics
* between neighboring polygons before returning the Model.
*
*
* @param filename  Path to file to be loaded
* @returns Model* Pointer to the active Model instance
*
* @throws std::bad_alloc  If the file cannot be saved inside the RAM
* @throws ModelLoadingException
*
* @see PLY format specification http://paulbourke.net/dataformats/ply/
*/
Model* ModelLoadingPly::load(std::string filename){
	parser.openFile(filename);

	Model* loaded = new Model(filename);
	vertexProperties.clear();

	try{
		readHeader(loaded);
		readVertices(loaded);
		readPolygons(loaded);
		readAdditionalEdges(loaded);
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

/***
* @brief Obtain model contents by reading the ASCII header of the file
*
* From the header we obtain the number of vertices and polygons that are stored
* in the file's body, as well as the specific type of data for each property
* field (following the PLY specifications), this information will be stored in
* the provided polygonMesh instance.
*
* Additional Edges are an extension that is currently supported by Camaron,
* their presence is included by using the keyword "edge"
*
* @todo support multiple types when reading property fields
*
* @param polygonMesh  Model instance that will store the header data
* @throws ModelLoadingException
*
* @see readAdditionalEdges() for details about the extension
*/
void ModelLoadingPly::readHeader(Model* model){

	numberOfBytesInVertexPropertiesToIgnore = 0;
	int vertNumber = 0, faceNumber = 0, edgeNumber = 0;
	bool readingVertexProperties = false;

	parser.setCurrentEncoding(Encoding::ASCII);
	std::string token;
	unsigned char fileEndianness = 0;
	bool isBinary = false;
	while(true){
		parser >> token;
		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached end of file before reading the body");
			}
		if (!token.compare("element")){
			parser >> token;
			if(!token.compare("vertex")){
				parser >> vertNumber;
				readingVertexProperties = true;
				}
			else if (!token.compare("face")){
				parser >> faceNumber;
				readingVertexProperties = false;
				}
			else if (!token.compare("edge")){
				parser >> edgeNumber;
				readingVertexProperties = false;
				}
			}
		else if(!token.compare("property")){
			//property name
			int propertySize = 0;
			if(readingVertexProperties){
				parser >> token;
				if(!token.compare("char") || !token.compare("uchar") ||
				   !token.compare("uint8") || !token.compare("int8")){
					propertySize = sizeof(char);
					}
				else if(!token.compare("int") || !token.compare("float") ||
						 !token.compare("uint")|| !token.compare("float32") ||
						 !token.compare( "int32")){
					propertySize = sizeof(int);
					}
				parser >> token;
				std::shared_ptr<ScalarFieldDef> scalarDef(new ScalarFieldDef(0,token, vertNumber));
				vertexProperties.push_back(scalarDef);
				if(token.compare("x") &&
				   token.compare("y") &&
				   token.compare("z")){
					//not a coordinate
					numberOfBytesInVertexPropertiesToIgnore += propertySize;
					}
				}
			}
		else if (!token.compare("end_header")){
			parser.prepareNextLine();
			break;
			}

		else if (!token.compare("format")){
			parser >> token;
			isBinary = token.compare("ascii") != 0;
			fileEndianness = token.compare("binary_big_endian") != 0;
			}

		parser.prepareNextLine();
		}

	///@note Who should handle this code? StreamScanner, Endianess or each Loader separately?
	unsigned char currentSystemEndianness = Endianess::findEndianness();
	if(isBinary){
		if (fileEndianness != currentSystemEndianness){
			parser.setCurrentEncoding(Encoding::EXTERNAL_BINARY);
			}
		else{
			parser.setCurrentEncoding(Encoding::NATIVE_BINARY);
			}
		}

	/// Reserve space based on the file expected contents
	model->reserveVertices(vertNumber);
	model->reservePolygons(faceNumber);
	model->reserveAdditionalEdges(edgeNumber);
	emit setupProgressBarForNewModel(vertNumber,faceNumber,0);
}

/***
* @brief Read a sequence of vertices in ASCII format
*
* Vertices are described using a sequence of numbers per row, usually the first
* three are used to store the vertex coordinates (x,y,z), but depending on the
* header contents, other variables can be included in the same row (normal
* orientation, color, etc). For this method in particular we will skip the
* non-coordinates fields.
*
* As vertices are read, the boundary box of the model is updated if a new
* vertex is located outside of it.
*
* This method only considers the vertex coordinates as data, other property
* fields contained in the same row are ignored as they are loaded separately
* by PropertyFieldLoading instances.
*
* @param   pol*	 Pointer to Model that will store the loaded vertices
* @throws ModelLoadingException
*/
void ModelLoadingPly::readVertices( Model* model ) {
	std::vector<vis::Vertex> &v = model->getVertices();
	std::vector<float> &bounds = model->getBounds();

	float x, y, z;
	for(unsigned int i = 0; i < v.capacity(); i++) {
		parser >> x >> y >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		v.emplace_back(i, x, y, z);
		updateBoundingBox(bounds, x, y, z);

		if(i%1000==0)
			emit setLoadedVertices(i);

		parser.prepareNextLine(numberOfBytesInVertexPropertiesToIgnore);
	}
	emit setLoadedVertices(model->getVerticesCount());
	model->getElementsRelations()->getPolygonsVertex().resize(v.capacity());

	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingPly] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
}

/***
* @brief Read a sequence of polygons
*
* An individual polygon is represented by a set of numbers separated by spaces,
* the first number represents the number of vertices of the polygon while the
* following numbers correspond to the row number containing the vertex properties.
*
* There is a risk of polygons having less sides than the expected number of reported
* vertices, in said case the program will raise a warning, skipping to the next line
*
* @param   pol* Pointer to the Model that will store the loaded polygons
* @throws ModelLoadingException
*/
void ModelLoadingPly::readPolygons(Model* model) {

	unsigned char vertsPerFace;
	unsigned int vertIndex;

	std::vector<vis::Vertex> &verticesMesh = model->getVertices();
	std::vector<vis::Polygon> &vecPol = model->getPolygons();
	ElementsRelations* relations = model->getElementsRelations();

	relations->getVertexPolygons().reserve(vecPol.capacity());
	relations->getPolygonsPolygons().resize(vecPol.capacity());

	int errors = 0;
	for(unsigned int i = 0; i < vecPol.capacity(); i++ ) {
		parser >> vertsPerFace;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested polygons");
			}
		vecPol.emplace_back(i-errors);

		relations->getVertexPolygons().emplace_back();
		relations->getVertexPolygonsById(i-errors).reserve(vertsPerFace);

		for(unsigned char j = 0; j < vertsPerFace; j++) {
			parser >> vertIndex;
			relations->addVertexInPolygon(i-errors, verticesMesh[vertIndex].getId());
			}

		if(relations->getVertexPolygonsById(i-errors).size()<3){
			vecPol.pop_back();
			errors++;
			relations->getVertexPolygonsById(i-errors).resize(0);
			}
		else{
			// getEdgesAndPolygons(model, relations->getVertexPolygonsById(i-errors), i-errors);
			if(vecPol.size()%5000==0)
				emit setLoadedPolygons(vecPol.size());
			}

		// getEdgesAndPolygons(model, relations->getVertexPolygonsById(i-errors), i-errors);
		parser.prepareNextLine();
		}

	if(errors>0)
		emit warningLoadingModel(QString("Ignored degenerated polygons: ") + QString::number(errors));

	emit setLoadedPolygons(vecPol.size());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	}


/***
* @brief Extract additional edges from a file if present
*
* Each additional edge row is described by the indexes of the vertices they
* connect, as well as three values representing its RGB color values.
* This rows are added at the end of the file after the polygon definitions.
*
* @param   pol*	 Pointer to the Model instance that will store the data
* @throws ModelLoadingException
*/
void ModelLoadingPly::readAdditionalEdges( Model* model ) {
	std::vector<vis::Edge> &e = model->getAdditionalEdges();

	model->getElementsRelations()->getVertexAdditionalEdges().reserve(e.capacity());

	int n0, n1;
	int r, g, b;
	for(unsigned int i = 0; i < e.capacity(); i++) {
		parser >> n0 >> n1 >> r >> g >> b;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested additional edges");
			}

		e.emplace_back(i, glm::vec3(r/255.0f,g/255.0f,b/255.0f));
		
		model->getElementsRelations()->getVertexAdditionalEdges().emplace_back(n0, n1);
		parser.prepareNextLine();
		}
	std::cout << "AdditionalEdgesVector: Capacity = " << e.capacity() << std::endl;
	std::cout << "AdditionalEdgesVector: Size = " << e.size() << std::endl;
	}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingPly);
