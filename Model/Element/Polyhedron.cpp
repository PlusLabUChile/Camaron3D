#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/SelectionStrategy.h"

vis::Polyhedron::Polyhedron( int newid ): Element( newid ){
	volumen = 0.0f;
	area = 0.0;
	geoCenter = glm::vec3(0,0,0);
}

vis::Polyhedron::~Polyhedron(){}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Polyhedron)

glm::vec3 vis::Polyhedron::getInwardNormal(vis::Polygon* p, vis::Vertex* v){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = geoCenter - v->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)<0)?
				-currentFaceNormal:currentFaceNormal;
}

glm::vec3 vis::Polyhedron::getOutwardNormal(vis::Polygon* p, vis::Vertex* v){
	glm::vec3 currentFaceNormal = p->getNormal();
	glm::vec3 inwardVector = geoCenter - v->getCoords();
	return (glm::dot(currentFaceNormal,inwardVector)>0)?
				-currentFaceNormal:currentFaceNormal;
}

///@note Should we keep this method or fully delegate it to EvaluationStrategy/PolyhedronArea.cpp ?
float vis::Polyhedron::getArea(){
	return area;
	}

void vis::Polyhedron::setArea(float value){
	area = value;
}

float vis::Polyhedron::getVolume(){
	return volumen;
}

void vis::Polyhedron::setVolumen(float value){
	volumen = value;
}

glm::vec3 vis::Polyhedron::getGeometricCenter(){
	return geoCenter;
}

void vis::Polyhedron::setGeometricCenter(glm::vec3 value){
	geoCenter = value;
}
