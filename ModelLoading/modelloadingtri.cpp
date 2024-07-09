#include "modelloadingtri.h"
#include "Utils/fileutils.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include <iostream>

ModelLoadingTri::ModelLoadingTri():ModelLoadingStrategy("Triangle Mesh","tri"){}

ModelLoadingTri::~ModelLoadingTri(){
	parser.closeFile();
	}

bool ModelLoadingTri::validate(std::string){
	return true;
	}

Model* ModelLoadingTri::load(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	Model* loaded = new Model(filename);

	try{
		readVertices(loaded);
		readPolygons(loaded);
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

void ModelLoadingTri::readVertices(Model* model){
	int vertexNumber;
	parser >> vertexNumber;
	parser.prepareNextLine();

	emit setupProgressBarForNewModel(vertexNumber, 0, 0);
	model->reserveVertices(vertexNumber);

	std::vector<vis::Vertex> &v = model->getVertices();
	std::vector<float> &bounds = model->getBounds();

	float x, y, z;
	for(int i = 0; i < vertexNumber; i++) {
		parser >> x >> y >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		v.emplace_back(i, x, y, z );
		updateBoundingBox(bounds, x , y, z);

		if(i%1000==0){
			emit setLoadedVertices(i);
			}
		parser.prepareNextLine();
		}

	emit setLoadedVertices(v.size());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;

#ifdef DEBUG_MOD
	std::cout << "[ModelLoadingOff] Model Bounds: \n\r";
	for( unsigned int i = 0; i < bounds.size(); i++ ) {
		std::cout << bounds[i] << "\n\r";
	}
#endif
	emit setLoadedVertices(vertexNumber);
	}

void ModelLoadingTri::readPolygons(Model* model) {
	int polygonNumber;
	parser >> polygonNumber;
	parser.prepareNextLine();

	std::vector<vis::Polygon> &vecPol = model->getPolygons();
	std::vector<vis::Vertex> &verticesMesh = model->getVertices();
	ElementsRelations* relations = model->getElementsRelations();
	
	model->reservePolygons(polygonNumber);
	relations->resizeVertexPolygons(polygonNumber);
	relations->resizePolygonsPolygons(polygonNumber);

	emit setupProgressBarForNewModel(verticesMesh.capacity(), polygonNumber,0);
	emit setLoadedVertices(verticesMesh.capacity());

	int v1, v2, v3;
	for(int i = 0; i < polygonNumber; i++) {
		parser >> v1 >> v2 >> v3;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested polygons");
			}

		vecPol.emplace_back(i);

		relations->addVertexInPolygon(i, v1);
		relations->addVertexInPolygon(i, v2);
		relations->addVertexInPolygon(i, v3);

		if(i%10000==0){
			emit setLoadedPolygons(i);
			}
		parser.prepareNextLine();
		}

	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	emit setLoadedPolygons(polygonNumber);
	}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingTri);
