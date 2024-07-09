#include "polygonpolygonneighborhoodworker.h"
#include <unordered_map>
#include "Model/Model.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"

#include <iostream>

PolygonPolygonNeighborhoodWorker::PolygonPolygonNeighborhoodWorker(Model * mesh,
																   unsigned int min,
																   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Obtain the Von Neumann vicinity of a range of polygons
*
* For each polygon in the mesh, identify the polygons that share a vertex with
* it and keep count of the number of shared vertices. If two polygons share
* two vertices (an edge), save the external polygon as a neighbor of the current one.
*
* Neighborhood information is stored as a vector inside each polygon instance,
* this information is accessed later through getNeighborPolygons, by the following
* classes:
*
* @see MinimumDihedralAngle and MaximumDihedralAngle (Evaluation)
* @see ExpandToNeighbors (Selection)
* @see ModelExportVisF
*/
void PolygonPolygonNeighborhoodWorker::doWork(){

	std::vector<vis::Polygon> &p = pmesh->getPolygons();
	ElementsRelations* relations = pmesh->getElementsRelations();
	
	for( unsigned int j = rangeMin; j < rangeMax; j++ ) {
		std::unordered_map<int,int> neighboringPolygons2;
		for (unsigned int index_vertex : relations->getVertexPolygonsById(p[j].getId())){
			for (unsigned int index_polygon: relations->getPolygonsVertexById(index_vertex)){
				if(index_polygon==((unsigned int)p[j].getId())) continue;
				if((++neighboringPolygons2[index_polygon]) == 2){
					relations->addNeighborPolygonInPolygon(p[j].getId(), index_polygon);
				}
			}
		}
	}
}