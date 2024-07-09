#include "Edge.h"
#include "Model/Element/Vertex.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/SelectionStrategy.h"

vis::Edge::Edge(int idn):
	Element(idn){}

vis::Edge::Edge(int idn, glm::vec3 color):
	Element(idn),
	color(color){
}

vis::Edge::~Edge(){}

void vis::Edge::setColor(glm::vec3 color) {
	this->color = color;
}
glm::vec3& vis::Edge::getColor() {
	return this->color;
}

bool vis::Edge::isAtSurface() {
	return true;
}

void vis::Edge::setGeometricCenter(glm::vec3 value){
	this->geoCenter = value;
}

glm::vec3 vis::Edge::getGeometricCenter() {
	return this->geoCenter;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Edge)
