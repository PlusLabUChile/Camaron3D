#include "Utils/PolygonUtils.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include "Utils/VertexUtils.h"
#include <iostream>

PolygonUtils::PolygonUtils() {}
PolygonUtils::~PolygonUtils() {}

void PolygonUtils::getTriangleIndices(Model* model, vis::Polygon* p,std::vector<GLuint>& indices){
	std::vector<unsigned int> &verticesIds = model->getElementsRelations()->getVertexPolygonsById(p->getId());
	unsigned int firstIndex = model->getVertices()[verticesIds[0]].getId();
	std::cout << firstIndex << " " << model->getVertices()[verticesIds[0]].getId() << std::endl;
	if(verticesIds.size() >= 3){
		for( std::vector<vis::Vertex*>::size_type i = 1; i < verticesIds.size() - 1; i++ ) {
			indices.push_back(firstIndex);
			indices.push_back(model->getVertices()[verticesIds[i]].getId());
			indices.push_back(model->getVertices()[verticesIds[i+1]].getId());
		}
	}
}
void PolygonUtils::configRVertexFlagAttribute(Model* model, vis::Vertex* element, RVertexFlagAttribute& attribute){
	if(element->getRmodelPositions().size()!=1u)
		attribute.enableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
	else
		attribute.disableFlag(RVertexFlagAttribute::VIRTUAL_VERTEX);
	if(VertexUtils::isVertexAtSurface(model, element))
		attribute.enableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
	else
		attribute.disableFlag(RVertexFlagAttribute::SURFACE_VERTEX);
}

void PolygonUtils::getTriangleVertices(Model* model, vis::Polygon* p, std::vector<RVertexFlagAttribute>& flagAttributes){
	std::vector<unsigned int> &verticesIds = model->getElementsRelations()->getVertexPolygonsById(p->getId());

	RVertexFlagAttribute virtualOEdge;
	virtualOEdge.enableFlag(RVertexFlagAttribute::VIRTUAL_OPOSITE_EDGE_FLAG);
	RVertexFlagAttribute realOEdge;
	if(PolygonUtils::isPolygonAtSurface(model, p)){ 
		virtualOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
		realOEdge.enableFlag(RVertexFlagAttribute::SURFACE_POLYGON);
	}
	int verticesPos = flagAttributes.size();
	std::vector<glm::vec3>::size_type verticesPosStart = verticesPos;
	ElementsRelations* relations = model->getElementsRelations();
	if(verticesIds.size() == 3){
		model->getVertex(verticesIds[0])->addRmodelPositions(verticesPos++);
		model->getVertex(verticesIds[1])->addRmodelPositions(verticesPos++);
		model->getVertex(verticesIds[2])->addRmodelPositions(verticesPos++);
		PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[0]),realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[1]),realOEdge);
		flagAttributes.push_back(realOEdge);
		PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[2]),realOEdge);
		flagAttributes.push_back(realOEdge);
	}
	else if(verticesIds.size() > 3){
		for( std::vector<vis::Vertex*>::size_type i = 1; i < verticesIds.size() - 1; i++ ) {
			model->getVertex(verticesIds[0])->addRmodelPositions(verticesPos++);
			model->getVertex(verticesIds[i])->addRmodelPositions(verticesPos++);
			model->getVertex(verticesIds[i+1])->addRmodelPositions(verticesPos++);
			if(i==1){
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[0]),realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i]),virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i+1]),realOEdge);
				flagAttributes.push_back(realOEdge);
			}
			else if((verticesIds.size()-2) == i){
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[0]),realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i]),realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i+1]),virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
			else{
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[0]),realOEdge);
				flagAttributes.push_back(realOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i]),virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
				PolygonUtils::configRVertexFlagAttribute(model, model->getVertex(verticesIds[i+1]),virtualOEdge);
				flagAttributes.push_back(virtualOEdge);
			}
		}
	}
	for(std::vector<glm::vec3>::size_type i = verticesPosStart ;i < flagAttributes.size(); ++i )
		p->addRmodelPositions(verticesPosStart++);
	std::vector<int>& rmodelPos = p->getRmodelPositions();
	int nTriangles = verticesIds.size() - 2;
	int mainTrianglePos = nTriangles/2*3;
	for(int i = 0;i<3;i++)
		flagAttributes[rmodelPos[mainTrianglePos+i]].
				enableFlag(RVertexFlagAttribute::REPRESENTS_POLYGON);
}

glm::vec3 PolygonUtils::calculateNormal(Model* model, vis::Polygon* p){
	ElementsRelations* relations = model->getElementsRelations();
	std::vector<unsigned int> &vecIds = relations->getVertexPolygonsById(p->getId());
	if( model->getVerticesCount() > 2 ) {
		glm::vec3 normal = glm::normalize(glm::cross(model->getVertex(vecIds[1])->getCoords() - model->getVertex(vecIds[0])->getCoords(),
										model->getVertex(vecIds[2])->getCoords() - model->getVertex(vecIds[1])->getCoords()));
		p->setNormal(normal);
		return normal;
	}
	return glm::vec3(0.0f,0.0f,0.0f);
}

float PolygonUtils::calculateArea(Model* model, vis::Polygon* p){
	float totalArea = 0.0f;
	std::vector<unsigned int>& verticesIds = model->getElementsRelations()->getVertexPolygonsById(p->getId());
	if(verticesIds.size() < 3){
		p->setArea(totalArea);
		return totalArea;
		}
	for(unsigned int i = 1; i < verticesIds.size() - 1; i++ ) {
		glm::vec3 result = glm::cross(model->getVertex(verticesIds[0])->getCoords() - model->getVertex(verticesIds[i])->getCoords(),
										model->getVertex(verticesIds[i])->getCoords() - model->getVertex(verticesIds[i+1])->getCoords());
		float normalDirection = glm::dot(result,p->getNormal());
		totalArea += ((normalDirection>0)?1.0:-1.0)*sqrt(glm::dot(result,result)) / 2.0;
		}

	float calculatedArea = (totalArea>0.0)?totalArea:-totalArea;
	p->setArea(calculatedArea);
	return calculatedArea;
}

float PolygonUtils::getDihedralAngle( vis::Polygon* p1,vis::Polygon* p2){
	float dotPo = glm::dot(p1->getNormal(),p2->getNormal());
	if(dotPo<-1.0f)
		dotPo = -1.0f;
	else if(dotPo > 1.0f)
		dotPo = 1.0f;
	return std::acos(dotPo);
}
glm::vec3 getBiggestAnglePoint(glm::vec3& p1,
							   glm::vec3& p2,
							   std::vector<glm::vec3>& otherPoints){
	glm::vec3 lastvec = p2-p1;
	typedef std::vector<glm::vec3>::size_type s_type;
	s_type pos = 0;
	float maxAngle = -std::numeric_limits<float>::max();
	for(s_type i = 0;i<otherPoints.size();i++){
		glm::vec3 testVec = otherPoints[i]-p1;
		float angle = glm::dot(lastvec,testVec)/(glm::length(testVec)*glm::length(lastvec));
		if(angle>maxAngle){
			pos = i;
			maxAngle = angle;
		}
	}
	glm::vec3 chosen = otherPoints[pos];
	otherPoints[pos] = otherPoints[otherPoints.size()-1];
	otherPoints.pop_back();
	return chosen;
}
glm::vec3 getCenter(std::vector<glm::vec3>& vec){
	glm::vec3 center(0.0f,0.0f,0.0f);
	for(glm::vec3 pos : vec )
		center+=pos;
	center/=(float)vec.size();
	return center;
}

void PolygonUtils::orderVerticesToCCWPolygon(std::vector<glm::vec3>& vec){
	if(vec.size()==0u)
		return;
	std::vector<glm::vec3> orderedCCW;
	orderedCCW.push_back(getCenter(vec));//fake point, must be removed later
	orderedCCW.push_back(vec[vec.size()-1]);
	vec.pop_back();
	while(vec.size()>0u)
		orderedCCW.push_back(getBiggestAnglePoint(orderedCCW[orderedCCW.size()-2],orderedCCW[orderedCCW.size()-1],vec));
	orderedCCW[0] = orderedCCW[orderedCCW.size()-1];
	orderedCCW.pop_back();
	vec.swap(orderedCCW);

}

bool PolygonUtils::isPolygonNeighbor(Model* model, vis::Polygon* p){
	for( unsigned int polID: model->getElementsRelations()->getPolygonsPolygonsById(p->getId()) ){
		if(polID == ((unsigned int) p->getId()))
			return true;
	}
	return false;
}

bool PolygonUtils::isPolygonAtSurface(Model* model, vis::Polygon* p){
	if(model->getPolyhedronsCount() == 0) return true;
	return model->getElementsRelations()->getPolyhedronPolygonsById(p->getId()).count == 1;
}

void PolygonUtils::invertVerticesOrder(Model* model, vis::Polygon* p){
	std::vector<unsigned int> tmp;
	std::vector<unsigned int>& vertices = model->getElementsRelations()->getVertexPolygonsById(p->getId());
	for(int i = (int)vertices.size()-1;i>=0;i--)
		tmp.push_back(vertices[i]);
	vertices.swap(tmp);
	// p->setNormal(p->getNormal() * glm::vec3(-1,-1,-1)); //invert normal
}

glm::vec3 PolygonUtils::calculateGeometricCenter(Model* model, vis::Polygon* p){
	glm::vec3 center(0.0f,0.0f,0.0f);
	std::vector<unsigned int>& vertices = model->getElementsRelations()->getVertexPolygonsById(p->getId());
	for( unsigned int vertID : vertices){
		vis::Vertex* vertex = &model->getVertices()[vertID];
		center += vertex->getCoords();
	}
	if(vertices.size()!=0u)
		center /= (float)vertices.size();
	p->setGeometricCenter(center);
	return center;
}