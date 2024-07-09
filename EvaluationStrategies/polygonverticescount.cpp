#include "polygonverticescount.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
PolygonVerticesCount::PolygonVerticesCount():
	PolygonEvaluation("Vertices Count", "Vertices Count")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.7f;
}

PolygonVerticesCount::~PolygonVerticesCount(){}

void PolygonVerticesCount::value(Model* model, vis::Polygon* m ) {
	float count = model->getElementsRelations()->getVertexPolygonsById(m->getId()).size();
	this->addNewValue(count);
	}

float PolygonVerticesCount::getNullValue(){
	return -1.0f;
}
void PolygonVerticesCount::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonVerticesCount);
