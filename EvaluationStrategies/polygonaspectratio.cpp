#include "polygonaspectratio.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include <QIcon>
PolygonAspectRatio::PolygonAspectRatio():
	PolygonEvaluation("Aspect Ratio","Aspect ratio is calculated dividing the shortest edge by the longest edge of the polygon.\n AR = EdgeMin/EdgeMax", "Aspect Ratio")
{
	weight = POLYGON_CRITERIA_WEIGHT_BASE + 0.6f;

}
PolygonAspectRatio::~PolygonAspectRatio(){}

void PolygonAspectRatio::value(Model* model, vis::Polygon* m){
	//std::vector<vis::Vertex*>& vertices = m->getVertices();
	std::vector<unsigned int>& verticesIds = model->getElementsRelations()->getVertexPolygonsById(m->getId());
	float min, max;
	int verticesSizeMinusOne = verticesIds.size()-1;

	min = glm::distance(model->getVertices()[verticesIds[verticesSizeMinusOne]].getCoords(),
						model->getVertices()[verticesIds[0]].getCoords());
	max = min;
	for(int i = 0; i < verticesSizeMinusOne; i++){
		float d = glm::distance(model->getVertices()[verticesIds[i]].getCoords(),model->getVertices()[verticesIds[i+1]].getCoords());
		min = std::min(min,d);
		max = std::max(max,d);
		}
	float ratio = min/max;
	this->addNewValue(ratio);
	}

float PolygonAspectRatio::getNullValue(){
	return -1.0f;
}

void PolygonAspectRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/pentagon16.ico");
}

#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(PolygonAspectRatio);
