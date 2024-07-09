#include "minimuminternalangle.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Utils/PolygonUtils.h"
#include <limits>
#include <QIcon>
#include <math.h>
#include <iostream>

MinimumInternalAngle::MinimumInternalAngle():
	PolygonEvaluation("Minimum Internal Angle", "Minimum Internal Angle")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.2f;
}
MinimumInternalAngle::~MinimumInternalAngle(){}

void MinimumInternalAngle::value(Model* model, vis::Polygon* m) {
	float minimumAngle = std::numeric_limits<float>::max();
	// std::vector<vis::Vertex*> &vec = m->getVertices();
	std::vector<unsigned int> &verticesIds = model->getElementsRelations()->getVertexPolygonsById(m->getId());
	if(verticesIds.size()<=2u){
		minimumAngle = 0.0;
		}
	else{
		for( decltype(verticesIds.size()) i = 0; i != verticesIds.size(); i++ ) {
			glm::vec3 u = model->getVertices()[verticesIds[(i+2)%verticesIds.size()]].getCoords() - model->getVertices()[verticesIds[(i+1)%verticesIds.size()]].getCoords();
			glm::vec3 v = model->getVertices()[verticesIds[(i)%verticesIds.size()]].getCoords() - model->getVertices()[verticesIds[(i+1)%verticesIds.size()]].getCoords();
			float dots = std::acos(glm::dot(u,v)/(glm::length(u)*glm::length(v)));
			minimumAngle = std::min(minimumAngle,dots);
			}
		}
	std::cout << minimumAngle << std::endl;
	this->addNewValue(minimumAngle);
	}

float MinimumInternalAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}
void MinimumInternalAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumInternalAngle);
