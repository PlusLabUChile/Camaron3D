#include "maximumdihedralangle.h"
#include "Model/Element/Polygon.h"
#include "Utils/PolygonUtils.h"
#include "ModelLoading/MeshProcessor.h"
#include "Model/Model.h"
#include <limits>
#include <QIcon>
#include <iostream>
#include <unordered_map>

MaximumDihedralAngle::MaximumDihedralAngle():
	PolygonEvaluation("Maximum Dihedral Angle", "Maximum Dihedral Angle")
{
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
}
MaximumDihedralAngle::~MaximumDihedralAngle(){}

void MaximumDihedralAngle::value(Model* model, vis::Polygon* m) {
	float maximumAngle = -std::numeric_limits<float>::max();
	std::vector<unsigned int> &idPolygons = model->getElementsRelations()->getPolygonsPolygonsById(m->getId());
	for( unsigned int idPol : idPolygons ) {
		float currentAngle = PolygonUtils::getDihedralAngle(m, &model->getPolygons()[idPol]);
		maximumAngle = std::max(currentAngle,maximumAngle);
	}
	this->addNewValue(maximumAngle);
}

float MaximumDihedralAngle::getNullValue(){
	return -std::numeric_limits<float>::max();
}

void MaximumDihedralAngle::evaluateElementsFrom(Model* model){
	if(model->getElementsRelations()->getPolygonsPolygons().size() == 0){
		MeshProcessor::completePolygonPolygonRelations(model);
	}
	applyEvaluation(model, model->getPolygons());
	this->applied = true;
}

void MaximumDihedralAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MaximumDihedralAngle);
