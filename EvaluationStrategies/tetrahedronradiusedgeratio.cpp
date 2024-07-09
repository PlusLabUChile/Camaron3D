#include "tetrahedronradiusedgeratio.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/polyhedronutils.h"
#include <QIcon>
#include <iostream>

TetrahedronRadiusEdgeRatio::TetrahedronRadiusEdgeRatio():
	PolyhedronEvaluation("Tetrahedron Radius Ratio", "Tetrahedron Radius Ratio")
{
	weight = POLYHEDRON_CRITERIA_WEIGHT_BASE + 2.0f;
}
TetrahedronRadiusEdgeRatio::~TetrahedronRadiusEdgeRatio(){}

void TetrahedronRadiusEdgeRatio::value(Model* model, vis::Polyhedron* m) {
	std::cout << "Iniciando value de TetrahedronRadiusEdgeRatio\n";
	ElementsRelations* relations = relations;
	std::vector<unsigned int> polygonIds = relations->getPolygonsPolyhedronsById(m->getId());

	if(polygonIds.size() != 4){
		std::cout << "no tiene cuatro vertices\n";
		this->addNewValue(this->getNullValue());
		return;
		}

	std::vector<unsigned int> verticesIds;
	PolyhedronUtils::getPolyhedronVertices(model, m, verticesIds);

	glm::vec3 a = model->getVertices()[relations->getVertexPolygonsById(m->getId())[verticesIds[0]]].getCoords();
	glm::vec3 b = model->getVertices()[relations->getVertexPolygonsById(m->getId())[verticesIds[1]]].getCoords();
	glm::vec3 c = model->getVertices()[relations->getVertexPolygonsById(m->getId())[verticesIds[2]]].getCoords();
	glm::vec3 d = model->getVertices()[relations->getVertexPolygonsById(m->getId())[verticesIds[3]]].getCoords();

	float div =	2* glm::determinant(glm::mat3(
					b-a,
					c-a,
					d-a));

	float top = glm::length(glm::pow(glm::length(d-a),2.0f)*glm::cross(b-a,c-a) +
							glm::pow(glm::length(c-a),2.0f)*glm::cross(d-a,b-a) +
							glm::pow(glm::length(b-a),2.0f)*glm::cross(c-a,d-a));
	float radius = glm::abs(top/div);
	std::cout << top << " / " << div << std::endl;

	float minLength = std::numeric_limits<float>::max();

	for( unsigned int polygonId : polygonIds ){
		std::vector<unsigned int> &verticesPolygon = relations->getVertexPolygonsById(polygonId);

		float polygonMinLength = std::min({glm::distance(model->getVertices()[verticesPolygon[0]].getCoords(), model->getVertices()[verticesPolygon[1]].getCoords()),
										   glm::distance(model->getVertices()[verticesPolygon[1]].getCoords(), model->getVertices()[verticesPolygon[2]].getCoords()),
										   glm::distance(model->getVertices()[verticesPolygon[2]].getCoords(), model->getVertices()[verticesPolygon[0]].getCoords())});
		minLength = std::min(minLength, polygonMinLength);
	}
	float ratio = radius/minLength;
	std::cout << radius << " " << minLength << "\n\n";
	this->addNewValue(ratio);
	}

float TetrahedronRadiusEdgeRatio::getNullValue(){
	return -std::numeric_limits<float>::max();
}

void TetrahedronRadiusEdgeRatio::QApplicationInitiatedEv(){
	qicon = new QIcon(":Images/UI/Images/EvaluationStrategyIcons/tetrahedron.png");
}
#include <Factories/EvaluationStrategyRegistry.h>
REGISTER_EVALUATION_STRATEGY(TetrahedronRadiusEdgeRatio);
