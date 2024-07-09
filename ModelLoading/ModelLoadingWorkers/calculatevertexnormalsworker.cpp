#include "calculatevertexnormalsworker.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include "Utils/PolygonUtils.h"
#include <iostream>
CalculateVertexNormalsWorker::CalculateVertexNormalsWorker(Model* mesh,
														   unsigned int min,
														   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Compute vertex normals for a range of vertices in the mesh
*
* Vertex normals are a used mainly by rendering classes, requiring the previous
* computation of face normals in the model (and polyhedron relations for polyhedron
* meshes). The vectors generated here will be stored in the VRAM later by RModel after
* the mesh processing pipeline is completed. This method sums the face normals
* of nearby polygons and normalizes the result.
*
* @note The normalization of a zero vector will result in undefined behavior,
* as glm::normalize will compute a float division by a zero, returning a NaN vector
* afterwards. This issue can occur if the normals of the neighboring polygons
* cancel each other due to alternating orientations.
*	To prevent portability issues, we compare the aggregation of normals
* with the zero vector before normalization, while this does not require an
* epsilon comparison between each vector value and 0.0f, it might cause issues
* if the vector values are extremely close to zero due to precision constraints.
*
* @todo Verify if the low precision issue can actually happen here with external
* models and refactor accordingly
*
* @todo See if there's a way to reduce the number of checks in this algorithm
*/
void CalculateVertexNormalsWorker::doWork(){
	std::vector<vis::Vertex>& vertices = pmesh->getVertices();
	for(unsigned int i = rangeMin; i < rangeMax; i++){
		glm::vec3 normal(0.0f, 0.0f, 0.0f);
		vis::Vertex* currentVertex = &vertices[i];
		std::vector<unsigned int>& vertexPolygons = pmesh->getElementsRelations()->getPolygonsVertexById(currentVertex->getId());
		int np = 0;
		if(!vertexPolygons.empty()){
			for( unsigned int polID : vertexPolygons ){
				vis::Polygon* polygon = &pmesh->getPolygons()[polID];
				if(PolygonUtils::isPolygonAtSurface(pmesh, polygon)){
					normal += polygon->getNormal();
					np++;
					}
				}
			if(np > 0 && (normal != glm::vec3(0.0f,0.0f,0.0f))){
				normal = glm::normalize(normal);
				}
			}
		currentVertex->setNormal(normal);
		}
	}

///@note If the normal comparison to zero ends up causing bottlenecks for
///		  massive files, replace the block with the one below and benchmark
/*
float length = glm::dot(normal,normal);
if(np > 0 && length != 0.0f){
	normal *= glm::inversesqrt(length);
	}
}
*/