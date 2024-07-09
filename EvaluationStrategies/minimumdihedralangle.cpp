#include "minimumdihedralangle.h"
#include "Model/Element/Polygon.h"
#include "Utils/PolygonUtils.h"
#include "Model/Model.h"
#include "ModelLoading/MeshProcessor.h"
#include <limits>
#include <QIcon>

MinimumDihedralAngle::MinimumDihedralAngle():
	PolygonEvaluation("Minimum Dihedral Angle", "Minimum Dihedral Angle")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.4f;
}
MinimumDihedralAngle::~MinimumDihedralAngle(){}

void MinimumDihedralAngle::value(Model* model, vis::Polygon* m) {
	float minimumAngle = std::numeric_limits<float>::max();
	std::vector<unsigned int> &idPolygons = model->getElementsRelations()->getPolygonsPolygonsById(m->getId());
	for( unsigned int idPol : idPolygons ) {
		float currentAngle = PolygonUtils::getDihedralAngle(m, &model->getPolygons()[idPol]);
		minimumAngle = std::min(minimumAngle,currentAngle);
		}
	this->addNewValue(minimumAngle);
}

float MinimumDihedralAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}

void MinimumDihedralAngle::evaluateElementsFrom(Model* model){
	if(model->getElementsRelations()->getPolygonsPolygons().size() == 0){
		MeshProcessor::completePolygonPolygonRelations(model);
	}
	applyEvaluation(model, model->getPolygons());
	this->applied = true;
}

void MinimumDihedralAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumDihedralAngle);
