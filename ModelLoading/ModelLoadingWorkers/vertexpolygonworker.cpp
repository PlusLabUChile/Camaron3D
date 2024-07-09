#include "vertexpolygonworker.h"
#include <unordered_map>
#include "Model/Model.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"


VertexPolygonWorker::VertexPolygonWorker(Model * mesh,
																   unsigned int min,
																   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Provides to each vertex ids of the polygons that belong
*
* This information is used to calculate normals for each vertex and to know if 
* a vertex is in the surface of a mesh (this information is used in SelectionStrategy and Rendering)
*/
void VertexPolygonWorker::doWork(){
	std::vector<vis::Polygon> &p = pmesh->getPolygons();
	ElementsRelations* relations = pmesh->getElementsRelations();

    for( unsigned int j = rangeMin; j < rangeMax; j++){
        vis::Polygon& polygon = p[j];
        for(unsigned int index_vertex : relations->getVertexPolygonsById(polygon.getId())){
            relations->addPolygonInVertex(index_vertex, polygon.getId());
        }
    }
}
