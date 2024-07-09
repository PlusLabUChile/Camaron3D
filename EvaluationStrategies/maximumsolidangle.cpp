#include "maximumsolidangle.h"
#include <limits>
#include "Utils/polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include <QIcon>
MaximumSolidAngle::MaximumSolidAngle():
	PolyhedronEvaluation("Maximum Solid Angle", "Maximum Solid Angle")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.4f;
}
MaximumSolidAngle::~MaximumSolidAngle(){}

void MaximumSolidAngle::value(Model* model, vis::Polyhedron* m) {
	float maximumAngle = -std::numeric_limits<float>::max();
	std::vector<unsigned int> verticesOfPolyhedronIDS;
	PolyhedronUtils::getPolyhedronVertices(model, m, verticesOfPolyhedronIDS);
	for( unsigned int vertex : verticesOfPolyhedronIDS ) {
		float currentAngle = PolyhedronUtils::getPolyhedronSolidAngleFromVertex(model, m, &model->getVertices()[vertex]);
		maximumAngle = std::max(maximumAngle,currentAngle);
		}
	this->addNewValue(maximumAngle);
	}

float MaximumSolidAngle::getNullValue(){
	return -std::numeric_limits<float>::max();
}
void MaximumSolidAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MaximumSolidAngle);
