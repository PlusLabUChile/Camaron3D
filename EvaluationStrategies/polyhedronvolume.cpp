#include "polyhedronvolume.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/polyhedronutils.h"
#include <QIcon>
PolyhedronVolume::PolyhedronVolume():
	PolyhedronEvaluation("Volume", "Volume")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.2f;
}

PolyhedronVolume::~PolyhedronVolume(){}

void PolyhedronVolume::value(Model* model, vis::Polyhedron* m) {
	float volumen = m->getVolume();
	if(volumen == 0) {PolyhedronUtils::calculateVolumen(model, m); volumen = m->getVolume();}
	this->addNewValue(volumen);
	}

float PolyhedronVolume::getNullValue(){
	return -1.0f;
}

void PolyhedronVolume::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolyhedronVolume);
