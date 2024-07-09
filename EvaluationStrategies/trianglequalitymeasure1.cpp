#include "trianglequalitymeasure1.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
TriangleQualityMeasure1::TriangleQualityMeasure1():
	PolygonEvaluation("Bhatia and Lawrence TM","Bhatia and Lawrence")
{
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
}

TriangleQualityMeasure1::~TriangleQualityMeasure1(){}

void TriangleQualityMeasure1::value(Model* model, vis::Polygon* m) {
	// triangles only
	std::vector<unsigned int> &verticesIds = model->getElementsRelations()->getVertexPolygonsById(m->getId());

	if(verticesIds.size() != 3){
		this->addNewValue(this->getNullValue());
		return;
		}

	float sideA = glm::length(model->getVertices()[verticesIds[0]].getCoords() - model->getVertices()[verticesIds[1]].getCoords());
	float sideB = glm::length(model->getVertices()[verticesIds[0]].getCoords() - model->getVertices()[verticesIds[2]].getCoords());
	float sideC = glm::length(model->getVertices()[verticesIds[1]].getCoords() - model->getVertices()[verticesIds[2]].getCoords());

	float area = m->getArea();
	float prop = 4*1.73205080757f*area/(sideA*sideA +
										sideB*sideB +
										sideC*sideC);
	this->addNewValue(prop);
	}

float TriangleQualityMeasure1::getNullValue(){
	return -1.0f;
}

void TriangleQualityMeasure1::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/triangle-outline-16.ico");
}

#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TriangleQualityMeasure1);
