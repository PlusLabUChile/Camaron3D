#include "MockModelLoading.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include <unordered_map>

MockModelLoading::MockModelLoading(){}
MockModelLoading::~MockModelLoading(){}

//Same as original implementation
void MockModelLoading::SEQcompleteVertexPolygonRelations(Model* pmesh){
	for(vis::Polygon& polygon : pmesh->getPolygons() ) {
		for( vis::Vertex* vertex : polygon.getVertices()){
			vertex->getVertexPolygons().push_back(&polygon);
			}
		}
	}

void MockModelLoading::SEQcompletePolygonPolygonRelations(Model* pmesh){
	for(vis::Polygon& polygon : pmesh->getPolygons()) {
		std::unordered_map<int,int> neighboringPolygons;
		std::vector<vis::Vertex*>& currentVertices = polygon.getVertices();
		std::vector<vis::Polygon*>& neighborPolygons = polygon.getNeighborPolygons();
		for( vis::Vertex* vertex : currentVertices ) {
			for( vis::Polygon* neighbor : vertex->getVertexPolygons() ){
				if(&polygon==neighbor)
					continue;
				if((++neighboringPolygons[neighbor->getId()])==2){
					neighborPolygons.push_back(neighbor);
					}
				}
			}
		}
	}

//Same as original implementation
void MockModelLoading::SEQcompletePolygonPolyhedronRelations(Model* pmesh){
	for(vis::Polyhedron& polyhedron : pmesh->getPolyhedrons()){
		for(vis::Polygon* polygon : polyhedron.getPolyhedronPolygons()){
			polygon->addPolyhedron(&polyhedron);
			}
		}
	}

void MockModelLoading::SEQcalculateNormalsPolygons(Model* pmesh){
	for(vis::Polygon& polygon : pmesh->getPolygons()){
		polygon.calculateNormal();
		}
	}

void MockModelLoading::SEQcalculateNormalsVertices(Model* pmesh){
	for(vis::Vertex& currentVertex : pmesh->getVertices()){
		glm::vec3 normal(0.0f,0.0f,0.0f);
		std::vector<vis::Polygon*>& vertexPolygons = currentVertex.getVertexPolygons();
		int np = 0;
		float length;
		if(!vertexPolygons.empty()){
			for(vis::Polygon* polygon : vertexPolygons ){
				if(polygon->isAtSurface()){
					normal += polygon->getNormal();
					np++;
					}
				}
			length = glm::dot(normal,normal);
			if(np > 0 && length != 0.0f){
				normal *= glm::inversesqrt(length);
				}
			}
		currentVertex.setNormal(normal);
		}
	}

void MockModelLoading::SEQcalculateGeoCenterPolyhedronMesh(Model* pmesh){
	for(vis::Polyhedron& polyhedron : pmesh->getPolyhedrons()){
		polyhedron.calculateGeoCenter();
	}
}
