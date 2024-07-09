#include "polyhedronutils.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Utils/PolygonUtils.h"
#include <algorithm>
#include <iostream>
PolyhedronUtils::PolyhedronUtils(){}
void addUniqueElement(unsigned int vertex, std::vector<unsigned int>& vertices){
	for( unsigned int v : vertices )
		if(v == vertex)
			return;//already in vector
	vertices.push_back(vertex);
}

void PolyhedronUtils::calculateArea(Model* model, vis::Polyhedron* polyhedron){
	float area = 0.0f;
	for(unsigned int polID : model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId()) ){
		vis::Polygon* polygon = &model->getPolygons()[polID];
		area += polygon->getArea();
		}
	polyhedron->setArea(area);
	}

void PolyhedronUtils::calculateVolumen(Model* model, vis::Polyhedron* polyhedron){
	float volumen = 0.0f;
	for(unsigned int polID : model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId()) ){
		vis::Polygon* polygon = &model->getPolygons()[polID];
		unsigned int firstVertexID = model->getElementsRelations()->getVertexPolygonsById(polygon->getId())[0];

		glm::vec3 inwardNormal = polyhedron->getInwardNormal(polygon, model->getVertex(firstVertexID));
		glm::vec3 inwardVectorF = polyhedron->getGeometricCenter() - model->getVertex(firstVertexID)->getCoords();

		//distance: distance from geoCenter to polygon plane
		float distance = glm::dot(inwardNormal,
								  inwardVectorF);//no need to divide by normal module (normal is normalized)
		volumen += distance*polygon->getArea()/3.0f;
		}
	polyhedron->setVolumen(volumen);
	}

void PolyhedronUtils::calculateGeometricCenter(Model* model, vis::Polyhedron* polyhedron){
	glm::vec3 geoCenter = glm::vec3(0.0f,0.0f,0.0f);
	int n = 0;
	for(unsigned int polID : model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId()) ){
		vis::Polygon* polygon = &model->getPolygons()[polID];
		for( unsigned int vertID : model->getElementsRelations()->getVertexPolygonsById(polygon->getId())){
			vis::Vertex* vertex = &model->getVertices()[vertID];
			geoCenter += vertex->getCoords();		
		}
		n += model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId()).size();
	}
	geoCenter /= (float)n;
	polyhedron->setGeometricCenter(geoCenter);
}

void PolyhedronUtils::getPolyhedronVertices(Model* model, vis::Polyhedron* p, std::vector<unsigned int>& ver){
	std::vector<unsigned int>& idsPolygon = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	for( unsigned int idPol : idsPolygon ){
		for( unsigned int vertex : model->getElementsRelations()->getVertexPolygonsById(idPol) )
			addUniqueElement(vertex,ver);
	}
}

void PolyhedronUtils::setPolyhedronRModelPositions(Model* model, vis::Polyhedron* p){
	std::vector<int>& rmodelPos = p->getRmodelPositions();
	std::vector<unsigned int> idsPolygons = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());
	for( unsigned int idPol : idsPolygons ){
		std::vector<int>& polygonRmodelPos = model->getPolygons()[idPol].getRmodelPositions();
		for( int position : polygonRmodelPos)
			rmodelPos.push_back(position);
	}
}

void PolyhedronUtils::getTetrahedronIndices(Model* model, vis::Polyhedron* p, std::vector<GLuint>& tetrahedronIds) {
	ElementsRelations* relations = model->getElementsRelations();
	std::vector<unsigned int> idsPolygons = relations->getPolygonsPolyhedronsById(p->getId());
	unsigned int mainVertexID = relations->getVertexPolygonsById(idsPolygons[0])[0];
	for(unsigned int i = 1; i < idsPolygons.size();i++){
		std::vector<unsigned int> verticesIDS = model->getElementsRelations()->getVertexPolygonsById(idsPolygons[i]);
		if(std::find(verticesIDS.begin(), verticesIDS.end(), mainVertexID) == verticesIDS.end()) {

            for(std::vector<vis::Vertex*>::size_type j = 1; j < verticesIDS.size()-1; j++) {
				tetrahedronIds.push_back(model->getVertex(mainVertexID)->getRmodelPositions()[0]);
				tetrahedronIds.push_back(model->getVertex(verticesIDS[0])->getRmodelPositions()[0]);
				tetrahedronIds.push_back(model->getVertex(verticesIDS[j])->getRmodelPositions()[0]);
				tetrahedronIds.push_back(model->getVertex(verticesIDS[j+1])->getRmodelPositions()[0]);
			}
		}
	}
}

float PolyhedronUtils::getPolyhedronSolidAngleFromVertex(Model* model, vis::Polyhedron* p, vis::Vertex* v){
	std::vector<unsigned int> projectedFaces;
	// std::vector<vis::Polygon*>& polygons = p->getPolyhedronPolygons();

	std::vector<unsigned int>& idsPolygons = model->getElementsRelations()->getPolygonsPolyhedronsById(p->getId());

	glm::vec3 geocenterToVertexV = v->getCoords() - p->getGeometricCenter();
	for( unsigned int idPol : idsPolygons ){
		unsigned int idVertex = model->getElementsRelations()->getVertexPolygonsById(idPol)[0];
		if(glm::dot(p->getOutwardNormal(&model->getPolygons()[idPol], model->getVertex(idVertex)),
					geocenterToVertexV)<0)
			projectedFaces.push_back(idPol);
	}
	float totalProjectedArea = 0.0f;
	for( unsigned int idPol : projectedFaces ){
		// std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		std::vector<unsigned int>& idVerticesPol = model->getElementsRelations()->getVertexPolygonsById(idPol);
		if( idVerticesPol.size() <= 2)
			continue;
		std::vector<glm::vec3> translatedCoords;
		for( unsigned int idVertex : idVerticesPol )
			translatedCoords.push_back(glm::normalize(model->getVertex(idVertex)->getCoords() - v->getCoords()));

		//For each vertex, its coordinates are projected to a sphere centered in v
		glm::vec3& v0(translatedCoords[0]);
		//divide polygon into triangles
		for(unsigned int j = 1; j < idVerticesPol.size()-1;j++){
			glm::vec3& v1(translatedCoords[j]);
			glm::vec3& v2(translatedCoords[j+1]);
			//get angle between vertices of the spherical triangle
			float angleA = std::acos(glm::dot(v0,v1)/( v0.length()*v1.length()));
			float angleB = std::acos(glm::dot(v0,v2)/( v0.length()*v2.length()));
			float angleC = std::acos(glm::dot(v1,v2)/( v1.length()*v2.length()));
			float angleAlfa = std::acos((std::cos(angleA)-(std::cos(angleB)*std::cos(angleC)))/
										(std::sin(angleB)*std::sin(angleC)));
			float ratio = std::sin(angleAlfa)/std::sin(angleA);
			float angleBeta = std::asin(ratio*std::sin(angleB));
			float angleGamma = std::asin(ratio*std::sin(angleC));
			float Et = angleAlfa + angleBeta + angleGamma - PI;//spherical excess in radians

			totalProjectedArea+=(Et/(4*PI));
		}
	}
	return totalProjectedArea;
}

glm::vec3 PolyhedronUtils::getCoordsSphericalCoordinates(glm::vec3 cartesian){
	glm::vec3 sphericalCoords;
	sphericalCoords.x = cartesian.length();
	if(cartesian.z==0)
		sphericalCoords.y = PI/2.0f;
	else if(cartesian.z<0)
		sphericalCoords.y = PI + std::atan(std::sqrt((cartesian.x*cartesian.x)+
													 (cartesian.y*cartesian.y))/cartesian.z);
	else
		sphericalCoords.y = std::atan(std::sqrt((cartesian.x*cartesian.x)+
												(cartesian.y*cartesian.y))/cartesian.z);

	if(cartesian.x>0&&cartesian.y>0)
		sphericalCoords.z = std::atan(cartesian.y/cartesian.x);
	else if(cartesian.x>0&&cartesian.y<0)
		sphericalCoords.z = 2*PI+std::atan(cartesian.y/cartesian.x);
	else if(cartesian.x==0)
		sphericalCoords.z = PI/2.0f*((cartesian.y > 0) - (cartesian.y < 0));
	else if(cartesian.x<0)
		sphericalCoords.z = PI+std::atan(cartesian.y/cartesian.x);
	return sphericalCoords;
}

bool PolyhedronUtils::isPolyhedronAtSurface(Model* model, vis::Polyhedron* polyhedron){
	for( unsigned int polID : model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId()) ){
		vis::Polygon* polygon = &model->getPolygons()[polID];
		if(PolygonUtils::isPolygonAtSurface(model, polygon))// if(polygon->isAtSurface())
			return true;
	}
	return false;
}
