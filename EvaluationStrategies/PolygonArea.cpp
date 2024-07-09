#include "EvaluationStrategies/PolygonArea.h"
#include "Utils/PolygonUtils.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
PolygonArea::PolygonArea():
	PolygonEvaluation("Area", "Area")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.1f;
}

PolygonArea::~PolygonArea(){}

void PolygonArea::value(Model* model, vis::Polygon* m ) {
	float area = m->getArea();
	if (area == 0.0f) {PolygonUtils::calculateArea(model, m); area = m->getArea();}
	this->addNewValue(area);
	}

float PolygonArea::getNullValue(){
	return -1.0f;
}

void PolygonArea::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonArea);
