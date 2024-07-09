#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Utils/PolygonUtils.h"

vis::Polygon::Polygon( int newid ):
	Element( newid ),
	normal(0.0f,0.0f,0.0f),
	area(0.0f),
	geoCenter(0.0f,0.0f,0.0f){}

vis::Polygon::~Polygon(){}

void vis::Polygon::setNormal(glm::vec3& value){
	this->normal = value;
}

glm::vec3& vis::Polygon::getNormal(){
	return this->normal;
}

void vis::Polygon::setArea(float value){
	this->area = value;
}

float vis::Polygon::getArea(){
	return this->area;
	}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Polygon)

glm::vec3 vis::Polygon::getGeometricCenter(){
	return this->geoCenter;
}

void vis::Polygon::setGeometricCenter(glm::vec3& value){
	this->geoCenter = value;
}
