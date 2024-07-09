#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Edge.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Exceptions/propertydefnotfoundexception.h"

Model::Model(std::string f): filename(f){

	bounds.resize(6);
	bounds[0] = bounds[1] = bounds[2] = std::numeric_limits<float>::max();
	bounds[3] = bounds[4] = bounds[5] = std::numeric_limits<float>::lowest();

	relations = new ElementsRelations();
	}

Model::~Model(){
	///@note Element vectors are stored by object, should not have issues here
	delete relations;
	}

std::vector<float>& Model::getBounds() {
	return bounds;
}

std::vector<vis::Vertex>& Model::getVertices(){
	return vertices;
	}

vis::Vertex* Model::getVertex(unsigned int vid){
	return &vertices[this->getElementsRelations()->getPositionInContainerById(vid)];
}

std::vector<vis::Edge>& Model::getAdditionalEdges(){
	return additionalEdges;
	}

std::vector<vis::Polygon>& Model::getPolygons(){
	return polygons;
	}

std::vector<vis::Polyhedron>& Model::getPolyhedrons(){
	return polyhedrons;
	}

ElementsRelations* Model::getElementsRelations(){
	return relations;
}

int Model::getVerticesCount(){
	return vertices.size();
	}

int Model::getAdditionalEdgesCount(){
	return additionalEdges.size();
	}

int Model::getPolygonsCount(){
	return polygons.size();
	}

int Model::getPolyhedronsCount(){
	return polyhedrons.size();
	}

void Model::reserveVertices(int n){
	vertices.reserve(n);
	}

void Model::reserveAdditionalEdges(int n){
	additionalEdges.reserve(n);
	}

void Model::reservePolygons(int n){
	polygons.reserve(n);
	}

void Model::reservePolyhedrons(int n){
	polyhedrons.reserve(n);
	}

std::string Model::getFilename(){
	return filename;
}

void Model::addPropertyFieldDef(std::shared_ptr<PropertyFieldDef> propertyFieldDef) {
	propertyFieldDefs.push_back(propertyFieldDef);
	propertyFieldPositions[propertyFieldDef.get()] = propertyFieldDefs.size()-1;
}

unsigned char Model::getPropertyFieldPosition(PropertyFieldDef* pfd) {
	if(!propertyFieldPositions.count(pfd)) {
		throw new PropertyDefNotFoundException;
	}
	return propertyFieldPositions[pfd];
}

std::vector<std::shared_ptr<PropertyFieldDef>>& Model::getPropertyFieldDefs() {
	return propertyFieldDefs;
}

///@deprecated Remove after reworking model

bool Model::is2D(){
	return _2D;
	}

void Model::set2D(bool b){
	_2D = b;
	}
