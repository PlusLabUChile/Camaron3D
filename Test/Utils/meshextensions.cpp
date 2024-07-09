#include "Test/Utils/meshextensions.h"

#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Edge.h"
#include "Model/Model.h"
#include <iostream>

MeshExtensions::MeshExtensions(){}
MeshExtensions::~MeshExtensions(){}

bool MeshExtensions::compare(vis::Vertex* first, vis::Vertex* second){
	return (first->getCoords() == second->getCoords()
			 && first->getNormal() == second->getNormal()
			 && first->getId() == second->getId());
	}

bool MeshExtensions::compare(vis::Edge* first, vis::Edge* second){
	return (compare(first->getVertex0(), second->getVertex0())
			 && compare(first->getVertex1(), second->getVertex1())
			 && first->getColor() == second->getColor());
	}

bool MeshExtensions::compare(vis::Polygon* first, vis::Polygon* second){

	std::vector<vis::Vertex*>& fvert = first->getVertices();
	std::vector<vis::Vertex*>& svert = second->getVertices();

	if (fvert.size() != svert.size()){
		return false;
		}

	for (unsigned int i = 0; i < fvert.size(); i++){
		if (!compare(fvert[i], svert[i])){
			return false;
			}
		}

	return (first->getNormal() == second->getNormal()
			 && first->getId() == second->getId());
	}

bool MeshExtensions::compare(vis::Polyhedron* first, vis::Polyhedron* second){
	std::vector<vis::Polygon*>& fpoly = first->getPolyhedronPolygons();
	std::vector<vis::Polygon*>& spoly = second->getPolyhedronPolygons();

	if (fpoly.size() != spoly.size()){
		return false;
		}

	for (unsigned int i = 0; i < fpoly.size(); i++){
		if (!compare(fpoly[i], spoly[i])){
			return false;
			}
		}

	return (first->getId() == second->getId());
	}

bool MeshExtensions::compare(Model* first, Model* second){
	if (first->getVerticesCount() != second->getVerticesCount()
		 || first->getAdditionalEdgesCount() != second->getAdditionalEdgesCount()){
		return false;
		}

	std::vector<vis::Vertex>& fvert = first->getVertices();
	std::vector<vis::Edge>& fedge = first->getAdditionalEdges();
	std::vector<vis::Vertex>& svert = second->getVertices();
	std::vector<vis::Edge>& sedge = second->getAdditionalEdges();

	for (unsigned int i = 0; i < fvert.size(); i++){
		if (!compare(&fvert[i], &svert[i])){
			return false;
			}
		}

	for (unsigned int i = 0; i < fedge.size(); i++){
		if (!compare(&fedge[i], &sedge[i])){
			return false;
			}
		}

	if (first->getBounds() != second->getBounds()){
		return false;
		}

	if (first->getPolygonsCount() != second->getPolygonsCount()){
		return false;
		}

	std::vector<vis::Polygon>& fpoly = first->getPolygons();
	std::vector<vis::Polygon>& spoly = second->getPolygons();
	for (unsigned int i = 0; i < fpoly.size(); i++){
		if (!compare(&fpoly[i], &spoly[i])){
			return false;
			}
		}

	if (first->getPolyhedronsCount() != second->getPolyhedronsCount()){
		return false;
		}

	std::vector<vis::Polyhedron>& fpolyh = first->getPolyhedrons();
	std::vector<vis::Polyhedron>& spolyh = second->getPolyhedrons();
	for (unsigned int i = 0; i < fpolyh.size(); i++){
		if (!compare(&fpolyh[i], &spolyh[i])){
			return false;
			}
		}

	return true;
	}
