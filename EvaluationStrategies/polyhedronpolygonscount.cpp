#include "polyhedronpolygonscount.h"
#include <QIcon>
#include "Model/Element/Polyhedron.h"
PolyhedronPolygonsCount::PolyhedronPolygonsCount():
	PolyhedronEvaluation("Faces Count", "Faces Count")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.9f;
}

PolyhedronPolygonsCount::~PolyhedronPolygonsCount(){}

void PolyhedronPolygonsCount::value(Model* model, vis::Polyhedron* m){
	float count = model->getElementsRelations()->getPolygonsPolyhedronsById(m->getId()).size();
	this->addNewValue(count);
	}

float PolyhedronPolygonsCount::getNullValue(){
	return -1.0f;
}

void PolyhedronPolygonsCount::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronPolygonsCount);
