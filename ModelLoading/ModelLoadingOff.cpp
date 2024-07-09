#include "ModelLoading/ModelLoadingOff.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include <iostream>
#include <string.h>
#include "Utils/hashingtree.h"


ModelLoadingOff::ModelLoadingOff():ModelLoadingStrategy("OFF", "off"){}

ModelLoadingOff::~ModelLoadingOff(){
	parser.closeFile();
	}

bool ModelLoadingOff::validate(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	std::string word;

	parser >> word;
	parser.closeFile();
	return !word.compare("OFF");
	}

Model* ModelLoadingOff::load(std::string filename){
	parser.setCurrentEncoding(Encoding::ASCII);
	parser.openFile(filename);

	Model* loaded = new Model(filename);

	try{
		readHeader(loaded);
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

void ModelLoadingOff::readHeader(Model* model){
	int vertexNumber = 0, faceNumber = 0;
	parser.prepareNextLine();	//skip initial OFF string

	parser >> vertexNumber >> faceNumber;

	model->reserveVertices(vertexNumber);
	model->reservePolygons(faceNumber);

	emit setupProgressBarForNewModel(vertexNumber, faceNumber, 0);
	parser.prepareNextLine();
	}

void ModelLoadingOff::readVertices(Model* model) {
	std::vector<vis::Vertex> &v = model->getVertices();
	std::vector<float> &bounds = model->getBounds();

	float x, y, z;
	for(unsigned int i = 0; i < v.capacity(); i++ ) {
		parser >> x >> y >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		v.emplace_back(i, x, y, z);
		updateBoundingBox(bounds, x , y, z);

		if(i%1000==0){
			emit setLoadedVertices(i);
			}

		parser.prepareNextLine();
		}

	emit setLoadedVertices(v.size());
	std::cout << "VertexVector: Capacity = " << v.capacity() << std::endl;
	std::cout << "VertexVector: Size = " << v.size() << std::endl;
	}

void ModelLoadingOff::readPolygons(Model* model) {
	int vertsPerFace, vertIndex;
	std::vector<vis::Polygon>& vecPol = model->getPolygons();
	std::vector<vis::Vertex> &verticesMesh = model->getVertices();
	ElementsRelations* relations = model->getElementsRelations();

	relations->getVertexPolygons().reserve(vecPol.capacity());

	for(unsigned int i = 0; i < vecPol.capacity(); i++) {
		parser >> vertsPerFace;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested polygons");
			}

		vecPol.emplace_back(i);
		relations->getVertexPolygons().emplace_back();
		
		for(int j = 0; j < vertsPerFace; j++){
			parser >> vertIndex;
			relations->addVertexInPolygon(i, vertIndex);
			}

		parser.prepareNextLine();
		if(i%1000==0){
			emit setLoadedPolygons(i);
			}
		}

	emit setLoadedPolygons(vecPol.size());
	std::cout << "PolygonVector: Capacity = " << vecPol.capacity() << std::endl;
	std::cout << "PolygonVector: Size = " << vecPol.size() << std::endl;
	}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingOff);

