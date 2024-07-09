#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Edge.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/PolygonUtils.h"
#include "Utils/polyhedronutils.h"
#include "Utils/matrixtransformationutils.h"
#include "Utils/shaderutils.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include <iostream>
RModel::RModel()
{
	positionDataBufferObject = NULL_BUFFER;
	vertexFlagsDataBufferObject = NULL_BUFFER;
	vertexNormalDataBufferObject = NULL_BUFFER;
	rmodelVertexPositionBufferObject = NULL_BUFFER;
	polygonPolyhedronIdsBufferObject = NULL_BUFFER;
	tetrahedronVertexIdsBufferObject = NULL_BUFFER;
	edgeVertexPositionsDataBufferObject = NULL_BUFFER;
	edgeColorDataBufferObject = NULL_BUFFER;

	kernel_positionDataBufferObject = NULL_BUFFER;
	kernel_vertexFlagsDataBufferObject = NULL_BUFFER;
	kernel_vertexNormalDataBufferObject = NULL_BUFFER;
	kernel_rmodelVertexPositionBufferObject = NULL_BUFFER;

	originalModel = (Model*)0;
	kernelComputed = false;
}

RModel::~RModel(){
	this->freeRAMFromVideoCardBuffer();
}

void RModel::deleteData(){
	this->freeRAMFromVideoCardBuffer();
	this->freeRAMKernelVideoCardBuffer();
	vertexFlagsAttribute.clear();
	std::vector<RVertexFlagAttribute>().swap(vertexFlagsAttribute);
	kernel_vertexFlagsAttribute.clear();
	std::vector<RVertexFlagAttribute>().swap(kernel_vertexFlagsAttribute);
	bounds.resize(6);
	bounds[0] = 0.0;
	bounds[1] = 0.0;
	bounds[2] = 0.0;
	bounds[3] = 1.0;
	bounds[4] = 1.0;
	bounds[5] = 1.0;
	originalModel = (Model*)0;
	currentRModelPropertyFieldDef = nullptr;
	kernelComputed = false;
}

Model* RModel::getOriginalModel(){
	return originalModel;
}

void RModel::copyModelBounds(Model *model)
{
    this->bounds.clear();
	this->bounds.reserve(model->getBounds().size());
	for( float bounds : model->getBounds())
		this->bounds.push_back(bounds);
}

void RModel::setKernelBuffers(RModel* krmodel){
	kernel_positionDataBufferObject = krmodel->positionDataBufferObject;
	kernel_vertexFlagsDataBufferObject = krmodel->vertexFlagsDataBufferObject;
	kernel_vertexNormalDataBufferObject = krmodel->vertexNormalDataBufferObject;
	kernel_rmodelVertexPositionBufferObject = krmodel->rmodelVertexPositionBufferObject;
	kernel_vertexFlagsAttribute = krmodel->vertexFlagsAttribute;
}

void RModel::loadRModelData(Model* model, CameraCamaron* camera){
	this->freeRAMFromVideoCardBuffer();
	vertexFlagsAttribute.clear();
	vertexFlagsAttribute.reserve(model->getVerticesCount());

	copyModelBounds(model);
	camera->setModelToCameraSpaceMatrix(MatrixTransformationUtils::generateModelToCameraSpaceMatrix(model->getBounds()));
	// If our model only contains vertices, we only need to upload the vertices vector to the GPU without modifications
	if (model->getPolygonsCount() == 0){
		uploadVerticesDirectly(model);
		}

	// Otherwise, the contents of RModelPosition will be dictated by the result of triangulating each face in the model.
	else {
		std::vector<vis::Polygon>& modelPolygons = model->getPolygons();
		for(vis::Polygon& polygon : modelPolygons){
			PolygonUtils::getTriangleVertices(model, &polygon,vertexFlagsAttribute);
			}
		///@note In this case, nVertices is not the number of vertices in the model!,
		/// instead its the value vertices coordinates required to represent the model after
		/// triangulation
		nVertices = vertexFlagsAttribute.size();
		loadVertexPositionAndNormals(model);
		loadVertexPolygonPolyhedronIds(model);
		if (model->getPolyhedronsCount() != 0){
			std::vector<vis::Polyhedron>& polyhedrons = model->getPolyhedrons();
			for(vis::Polyhedron& polyhedron : polyhedrons){
				PolyhedronUtils::setPolyhedronRModelPositions(model, &polyhedron);
				}
			loadTetrahedronVertexIds(model);
			}
		}

	vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(vertexFlagsAttribute);
	if(model->getAdditionalEdgesCount() != 0){
		loadAdditionalEdges(model);
		}

	originalModel = model;
	}

void RModel::loadKernelRModelData(Model* model){
	this->freeRAMKernelVideoCardBuffer();
	kernel_vertexFlagsAttribute.clear();
	kernel_vertexFlagsAttribute.reserve(model->getVerticesCount());
	std::vector<vis::Polygon>& modelPolygons = model->getPolygons();
	for(vis::Polygon& polygon : modelPolygons){
		PolygonUtils::getTriangleVertices(model, &polygon, kernel_vertexFlagsAttribute);
		}
	nVertices = kernel_vertexFlagsAttribute.size();
	std::vector<glm::f64vec3> vecContainer;
	vecContainer.resize(nVertices);
	std::vector<vis::Vertex>& vertices = model->getVertices();
	//coords
	for( vis::Vertex& vertex : vertices ){
		for( int position : vertex.getRmodelPositions() )
			vecContainer[position] = vertex.getCoords();
	}
	kernel_positionDataBufferObject = ShaderUtils::createDataBuffer<glm::f64vec3>(vecContainer);
	//normals
	for( vis::Vertex& vertex : vertices ){
		for(int position : vertex.getRmodelPositions())
			vecContainer[position] = vertex.getNormal();
	}
	kernel_vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::f64vec3>(vecContainer);
	kernel_vertexFlagsDataBufferObject = ShaderUtils::createDataBuffer<RVertexFlagAttribute>(kernel_vertexFlagsAttribute);
	kernelComputed = true;
}

bool RModel::isKernelComputed(){
	return kernelComputed;
}

void RModel::setKernelComputed(bool value){
	kernelComputed = value;
}

void RModel::uploadVerticesDirectly(Model* model){
	RVertexFlagAttribute flag;
	flag.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);

	std::vector<glm::vec3> vertices;
	vertices.reserve(model->getVerticesCount());

	for(vis::Vertex& vertex : model->getVertices()) {
		vertices.push_back(vertex.getCoords());
		vertexFlagsAttribute.push_back(flag);
		}

	nVertices = model->getVerticesCount();
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vertices);
	}

void RModel::loadAdditionalEdges(Model* model){
	std::vector<glm::vec3> edgeContainer;
	std::vector<glm::vec3> edgeColorContainer;
	std::vector<vis::Edge>& additionalEdges = model->getAdditionalEdges();
	edgeContainer.reserve(model->getAdditionalEdgesCount()*2);
	edgeColorContainer.reserve(model->getAdditionalEdgesCount()*2);

	for( vis::Edge& edge : additionalEdges ){
		unsigned int vec0ID = model->getElementsRelations()->getVertexAdditionalEdgesById(edge.getId()).first_value;
		unsigned int vec1ID = model->getElementsRelations()->getVertexAdditionalEdgesById(edge.getId()).second_value;

		glm::vec3& vec0 = model->getVertices()[vec0ID].getCoords();
		glm::vec3& vec1 = model->getVertices()[vec1ID].getCoords();
		glm::vec3& color = edge.getColor();

		edgeContainer.push_back(vec0);
		edgeContainer.push_back(vec1);
		edgeColorContainer.push_back(color);
		edgeColorContainer.push_back(color);
	}
	nAdditionalEdges = model->getAdditionalEdgesCount();
	edgeVertexPositionsDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(edgeContainer);
	edgeColorDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(edgeColorContainer);
}

void RModel::loadVertexPositionAndNormals(Model* model){
	std::vector<glm::vec3> vecContainer;
	vecContainer.resize(nVertices);

	std::vector<vis::Vertex>& vertices = model->getVertices();
	//coords
	for( vis::Vertex& vertex : vertices ){
		for( int position : vertex.getRmodelPositions() )
			vecContainer[position] = vertex.getCoords();
	}
	positionDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vecContainer);
	//normals
	for( vis::Vertex& vertex : vertices ){
		for(int position : vertex.getRmodelPositions())
			vecContainer[position] = vertex.getNormal();
	}
	vertexNormalDataBufferObject = ShaderUtils::createDataBuffer<glm::vec3>(vecContainer);
}

std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> RModel::loadPropertyField(std::shared_ptr<ScalarFieldDef> pfd){
	Model* model = originalModel;
	// First check if the current property field is the one we're asked for
	if(currentRModelPropertyFieldDef && currentRModelPropertyFieldDef->getPropertyFieldDef()
			== std::dynamic_pointer_cast<PropertyFieldDef>(pfd)) {
		return currentRModelPropertyFieldDef;
	}
	// Delete the currently loaded property field
	currentRModelPropertyFieldDef = nullptr;
	// Load the new RModelPropertyFieldDef
	unsigned char pfdposition = model->getPropertyFieldPosition(pfd.get());
	std::vector<float> propertyData = model->getPropertyFieldDefs()[pfdposition]->getPropertyData();
	std::vector<float> floatContainer;
	floatContainer.resize(nVertices* pfd->getElementSize());
	std::vector<vis::Vertex>& vertices = model->getVertices();

	//coords
	for (unsigned int i = 0; i < vertices.size(); i++){
		std::vector<int>& rmodelPos = vertices[i].getRmodelPositions();
		for(int pos : rmodelPos){
			floatContainer[pos] = propertyData[i];
			}
		}
	GLuint buffer = ShaderUtils::createDataBuffer<float>(floatContainer);
	currentRModelPropertyFieldDef = std::make_shared<RModelPropertyFieldDef<PropertyFieldDef>>(
													std::dynamic_pointer_cast<PropertyFieldDef>(pfd),
													buffer,
													0,
													(GLvoid*)0);
	return currentRModelPropertyFieldDef;
}

void RModel::loadVertexPolygonPolyhedronIds(Model* model){
	std::vector<int> ids;
	std::vector<vis::Polygon>& polygons = model->getPolygons();

	for( vis::Polygon& polygon : polygons ){
		int npoltriangles = polygon.getRmodelPositions().size()/3;
		int id0 = polygon.getId(), id1 = -1, id2 = -1;
		if (model->getPolyhedronsCount() != 0){
			relationPair& neighbors = model->getElementsRelations()->getPolyhedronPolygonsById(polygon.getId());			
			id1 = (neighbors.count == 1)?neighbors.first_value:-1;
			id2 = (neighbors.count == 2)?neighbors.second_value:-1;
		}
		for(int j = 0;j<npoltriangles;j++){
			ids.push_back(id0);
			ids.push_back(id1);
			ids.push_back(id2);
		}
	}
	numberOfTriangles = ids.size()/3;
	polygonPolyhedronIdsBufferObject = ShaderUtils::createDataBuffer<int>(ids);
	for(int i = 0;i<nVertices;++i)
		ids[i]=i;
	rmodelVertexPositionBufferObject = ShaderUtils::createDataBuffer<int>(ids);
	std::vector<vis::Vertex>& modelVertices = model->getVertices();
	for( vis::Vertex& vertex : modelVertices ){
		for(int position : vertex.getRmodelPositions()){
			ids[position] = vertex.getId();
		}
	}
	vertexIdsBufferObject = ShaderUtils::createDataBuffer<int>(ids);
}

void RModel::loadTetrahedronVertexIds(Model* model) {
	std::vector<GLuint> ids;
	std::vector<vis::Polyhedron>& polyhedrons = model->getPolyhedrons();
	for(vis::Polyhedron& polyhedron : polyhedrons) {
		PolyhedronUtils::getTetrahedronIndices(model, &polyhedron, ids);
	}
	numberOfTetrahedrons = ids.size()/4;
	tetrahedronVertexIdsBufferObject = ShaderUtils::createDataBuffer<GLuint>(ids);
}

void RModel::refreshSelectedElements(){
	ShaderUtils::setDataBuffer<RVertexFlagAttribute>(vertexFlagsDataBufferObject,vertexFlagsAttribute);
}

void RModel::freeRAMFromVideoCardBuffer(){
	glDeleteBuffers(1,&this->vertexFlagsDataBufferObject);
	glDeleteBuffers(1,&this->positionDataBufferObject);
	glDeleteBuffers(1,&this->vertexNormalDataBufferObject);
	if(currentRModelPropertyFieldDef) {
		GLuint rModelPropertyFieldDefBuffer = currentRModelPropertyFieldDef->getBuffer();
		glDeleteBuffers(1,&rModelPropertyFieldDefBuffer);
	}
	glDeleteBuffers(1,&rmodelVertexPositionBufferObject);
	glDeleteBuffers(1,&polygonPolyhedronIdsBufferObject);
	glDeleteBuffers(1,&tetrahedronVertexIdsBufferObject);
	glDeleteBuffers(1,&edgeVertexPositionsDataBufferObject);
	glDeleteBuffers(1,&edgeColorDataBufferObject);
	this->positionDataBufferObject = RModel::NULL_BUFFER;
	this->vertexFlagsDataBufferObject = RModel::NULL_BUFFER;
	this->vertexNormalDataBufferObject = RModel::NULL_BUFFER;
	rmodelVertexPositionBufferObject = RModel::NULL_BUFFER;
	polygonPolyhedronIdsBufferObject = RModel::NULL_BUFFER;
	tetrahedronVertexIdsBufferObject = RModel::NULL_BUFFER;
	edgeVertexPositionsDataBufferObject = RModel::NULL_BUFFER;
	edgeColorDataBufferObject = RModel::NULL_BUFFER;
}

void RModel::freeRAMKernelVideoCardBuffer(){
	glDeleteBuffers(1,&this->kernel_vertexFlagsDataBufferObject);
	glDeleteBuffers(1,&this->kernel_positionDataBufferObject);
	glDeleteBuffers(1,&this->kernel_vertexNormalDataBufferObject);
	glDeleteBuffers(1,&kernel_rmodelVertexPositionBufferObject);
	this->kernel_positionDataBufferObject = RModel::NULL_BUFFER;
	this->kernel_vertexFlagsDataBufferObject = RModel::NULL_BUFFER;
	this->kernel_vertexNormalDataBufferObject = RModel::NULL_BUFFER;
	this->kernel_rmodelVertexPositionBufferObject = RModel::NULL_BUFFER;
}

void RModel::setBackgroundColor(glm::vec4 s){
	backgroundColor = s;
}

glm::vec4 RModel::getBackgroundColor(){
	return backgroundColor;
}