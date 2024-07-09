#include "EvaluationStrategies/PolyhedronArea.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>
PolyhedronArea::PolyhedronArea():
	PolyhedronEvaluation("Surface","Surface")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.1f;
}
PolyhedronArea::~PolyhedronArea(){}

void PolyhedronArea::value(Model* model, vis::Polyhedron* m) {
	float totalArea = 0.0;
	std::vector<unsigned int> polygonsIds = model->getElementsRelations()->getPolygonsPolyhedronsById(m->getId());
	for( unsigned int polId : polygonsIds ) {
		float areaPolygon = model->getPolygons()[polId].getArea();
		totalArea += areaPolygon;
	}
	this->addNewValue(totalArea);
}

float PolyhedronArea::getNullValue(){
	return -1.0f;
}

void PolyhedronArea::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronArea);
