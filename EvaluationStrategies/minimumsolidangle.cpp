#include "minimumsolidangle.h"
#include <limits>
#include "Model/Model.h"
#include "Utils/polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"

#include <QIcon>
MinimumSolidAngle::MinimumSolidAngle():
	PolyhedronEvaluation("Minimum Solid Angle", "Minimum Solid Angle")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 0.3f;
}
MinimumSolidAngle::~MinimumSolidAngle(){}

void MinimumSolidAngle::value(Model* model, vis::Polyhedron* m) {
	float minimumAngle = std::numeric_limits<float>::max();
	std::vector<unsigned int> verticesOfPolyhedronIDS;
	PolyhedronUtils::getPolyhedronVertices(model, m, verticesOfPolyhedronIDS);
	for( unsigned int vertex : verticesOfPolyhedronIDS ) {
		float currentAngle = PolyhedronUtils::getPolyhedronSolidAngleFromVertex(model, m, &model->getVertices()[vertex]);
		minimumAngle = std::min(minimumAngle,currentAngle);
		}
	this->addNewValue(minimumAngle);
	}

float MinimumSolidAngle::getNullValue(){
	return std::numeric_limits<float>::max();
}
void MinimumSolidAngle::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/cube16.ico");
}

#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(MinimumSolidAngle);
