#include "ModelLoading/modelloadingsmesh.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/hashingtree.h"
#include "Utils/fileutils.h"
#include <iostream>
#include <limits>

ModelLoadingSmesh::ModelLoadingSmesh():ModelLoadingStrategy(){
	acceptedFileFormats.push_back(AcceptedFileFormat("SMESH","smesh"));
}

ModelLoadingSmesh::~ModelLoadingSmesh(){
    parser.closeFile();
}

bool ModelLoadingSmesh::validate(std::string filename){
	std::string filenameExtension = FileUtils::getFileNameExtension(filename);
	if(filenameExtension.compare("smesh") == 0){
		std::string filenameNoExtension = FileUtils::getFileNameWithoutExtension(filename);
		if(FileUtils::fileExists(filenameNoExtension+".smesh"))
			return true;
		}
	return false;
}

Model* ModelLoadingSmesh::load(std::string filename){
    std::string filenameNoExtension = FileUtils::getFileNameWithoutExtension(filename);
    path = filenameNoExtension;
    parser.setCurrentEncoding(Encoding::ASCII);
    parser.openFile(filename);
    Model* model = new Model(filename);
    try{
        readHeaderSmesh(model);
        readPolygons(model);
        parser.closeFile();
        return model;
    }
    catch(std::bad_alloc &ba){
        if(model) delete model;
        parser.closeFile();
        throw std::bad_alloc();
    }
    catch(ModelLoadingException ex){
        delete model;
        parser.closeFile();
        throw ex;
    }
}

void ModelLoadingSmesh::readHeaderSmesh(Model* model){
    parser >> numberOfNodes >> dimensions >> numberOfAttributesPerNode >> numberOfBoundaryMarkersNode;
    std::cout << numberOfNodes << " " << dimensions << std::endl;
    if (dimensions != 3)
        throw ModelLoadingException(path, "Dimension is not a valid value, 3 is expected");
    if(numberOfNodes == 0){
        parser.closeFile();
        readVerticesNode(model);
        parser.openFile(path+".smesh");
        parser.prepareNextLine();
    }
    else{
        readVerticesSmesh(model);
    }
        
}

void ModelLoadingSmesh::readVerticesSmesh(Model* model){
    std::vector<vis::Vertex>& vertices = model->getVertices();
    std::vector<float>& bounds = model->getBounds();
    ElementsRelations* relations = model->getElementsRelations();

    int index;
    float x = 0.0f, y = 0.0f, z = 0.0f;
    model->reserveVertices(numberOfNodes);
    for(int i = 0; i < numberOfNodes; i++){
        parser >> index >> x >> y >> z;
        
        if(parser.invalidState())
            throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
        
        vertices.emplace_back(index, x, y, z);
        updateBoundingBox(bounds, x, y, z);

        if(i == 0) model->getElementsRelations()->setDiffVertex(!(i == index));
        if(i%1000 == 0) emit setLoadedVertices(i);

        // Code for attributes when numberOfAttributesNode is not equal to 0

        parser.prepareNextLine();
    }

	emit setLoadedVertices(vertices.size());
}

void ModelLoadingSmesh::readVerticesNode(Model* model){
    bool value = parser.openFile(path+".node");
	parser >> numberOfNodes >> dimensions >> numberOfAttributesPerNode >> numberOfBoundaryMarkersNode;
    std::cout << value << std::endl;

	std::vector<vis::Vertex>& vertices = model->getVertices();
	std::vector<float> &bounds = model->getBounds();
	ElementsRelations* relations = model->getElementsRelations();

	int index;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	vertices.reserve(numberOfNodes);
	for(int i = 0; i< numberOfNodes; i++){
		parser >> index >> x >> y;
		if(dimensions>2)
			parser >> z;

		if (parser.invalidState()){
			throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested vertices");
			}

		vertices.emplace_back(index, x, y, z);
		updateBoundingBox(bounds, x, y, z);

		if(i==0) model->getElementsRelations()->setDiffVertex(!(i == index));
		if(i%5000==0)
			emit setLoadedVertices(i);

		parser.prepareNextLine();
		}

	relations->checkVertices();

	parser.closeFile();
	emit setLoadedVertices(numberOfNodes);
    dimensions = 3;
}

void ModelLoadingSmesh::readPolygons(Model* model){
    int verticesPerFace, vertexIndex;
    std::vector<vis::Vertex>& vertices = model->getVertices();
    std::vector<vis::Polygon>& polygons = model->getPolygons();
    ElementsRelations* relations = model->getElementsRelations();

    parser >> numberOfFaces >> numberOfBoundaryMarkersFace;

    for(int i = 0; i < numberOfFaces; i++){
        parser >> verticesPerFace;
        if (parser.invalidState()) throw ModelLoadingException(model->getFilename(), "ERROR: Reached EOF before reading all requested polygons");

        polygons.emplace_back(i);
        relations->getVertexPolygons().emplace_back();
        
        for(int j = 0; j < verticesPerFace; j++){
            parser >> vertexIndex;
            relations->addVertexInPolygon(i, vertexIndex);
        }

        parser.prepareNextLine();
        if(i%1000 == 0) emit setLoadedPolygons(i);

    }
    emit setLoadedPolygons(numberOfFaces);
    
}

#include "Factories/ModelLoadingFactory.h"
REGISTER_MODEL_LOADING_STRATEGY(ModelLoadingSmesh);

