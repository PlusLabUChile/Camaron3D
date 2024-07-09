#include "triangleradiusratio.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include <QIcon>
TriangleRadiusRatio::TriangleRadiusRatio():
	PolygonEvaluation("Radius Ratio","Radius Ratio")
{
	weight = TRIANGLE_CRITERIA_WEIGHT_BASE + 0.5f;
	descriptionWidget = 0;
}
TriangleRadiusRatio::~TriangleRadiusRatio(){
	if(descriptionWidget)
		delete descriptionWidget;
}
void TriangleRadiusRatio::value(Model* model, vis::Polygon* t) {
	// triangles only
	std::vector<unsigned int> &verticesIds = model->getElementsRelations()->getVertexPolygonsById(t->getId());

	if(verticesIds.size() != 3){
		this->addNewValue(this->getNullValue());
		return;
		}

	float sideA = glm::length(model->getVertices()[verticesIds[0]].getCoords() - model->getVertices()[verticesIds[1]].getCoords());
	float sideB = glm::length(model->getVertices()[verticesIds[0]].getCoords() - model->getVertices()[verticesIds[2]].getCoords());
	float sideC = glm::length(model->getVertices()[verticesIds[1]].getCoords() - model->getVertices()[verticesIds[2]].getCoords());
	
	float radiusRatio = 16*t->getArea()*t->getArea()/(sideA*sideB*sideC*(sideA+sideB+sideC));
	this->addNewValue(radiusRatio);
	}

float TriangleRadiusRatio::getNullValue(){
	return -1.0f;
}

bool TriangleRadiusRatio::hasDescriptionQWidget(){
	return true;
}

QWidget* TriangleRadiusRatio::getEvaluationStrategyDescriptionQWidget(){
	return descriptionWidget;
}
void TriangleRadiusRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/triangle-outline-16.ico");
	descriptionWidget = new QWidget((QWidget*)0,
									Qt::Tool| Qt::Window | Qt::CustomizeWindowHint| Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
}


#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TriangleRadiusRatio);

