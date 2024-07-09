#ifndef FIXSURFACEPOLYGONSVERTICESORDERWORKER_H
#define FIXSURFACEPOLYGONSVERTICESORDERWORKER_H

#include "Model/Model.h"
#include "Utils/qtworkerbase.h"
#include "Utils/PolygonUtils.h"
class PolyhedronMesh;
template<class POLYGON_TYPE, class POLYHEDRON_TYPE>
class FixSurfacePolygonsVerticesOrderWorker: public QtWorkerBase
{
	public:
		FixSurfacePolygonsVerticesOrderWorker(Model* mesh,
											  unsigned int min,
											  unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

template<class POLYGON_TYPE, class POLYHEDRON_TYPE>
FixSurfacePolygonsVerticesOrderWorker<POLYGON_TYPE,POLYHEDRON_TYPE>::
FixSurfacePolygonsVerticesOrderWorker(Model* mesh,
									  unsigned int min,
									  unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Attempt to correct surface face normals that are pointing inwards
*
* To accomplish this, the worker will pick each polygon in the given range,
* if its in the surface and its part of an existing polyhedron, use the geometric
* center of the latter to see if the polygon normal is pointing towards this point.
*
* In said case, the algorithm proceeds to invert the normal and reverse the list
* of component vertices of the polygon (in the case of recomputing the normals,
* the result will be pointing outwards).
*
* @note This step requires the previous computation of polygon polyhedron
* relationships, polyhedron geocenters and face normals.
*
* @note Using this worker on polygon meshes will do nothing, as no polyhedrons can
* be used to determine where the "outside" of the model is located.
*
* @note Loading models with incorrect normals is an expected use case, Camaron
* should aid in the correction of the issue by displaying them on the renderer
* and giving the user the choice to correct them. This means that incorrect normals
* should not be corrected during the initial mesh processing pipeline, delegating
* the use of this worker to a user request after loading.
*
* @todo Implement on-demand normal correction and use this worker to do so.
*/

template<class POLYGON_TYPE, class POLYHEDRON_TYPE>
void FixSurfacePolygonsVerticesOrderWorker<POLYGON_TYPE,POLYHEDRON_TYPE>::
doWork(){
	std::vector<POLYGON_TYPE>& polygons = pmesh->getPolygons();
	for(unsigned int i = rangeMin;i<rangeMax;i++){
		POLYGON_TYPE& current = polygons[i];
		if(PolygonUtils::isPolygonAtSurface(pmesh, &current)){
			relationPair& polyhedrons = pmesh->getElementsRelations()->getPolyhedronPolygonsById(current.getId());
			// POLYHEDRON_TYPE* pol = current.getNeighborPolyhedron((POLYHEDRON_TYPE*)0);
			if(polyhedrons.count != 0){
				POLYHEDRON_TYPE* pol = &pmesh->getPolyhedrons()[polyhedrons.first_value];
				unsigned int firstVertexID = pmesh->getElementsRelations()->getVertexPolygonsById(current.getId())[0];
				glm::vec3 currentFaceNormal = current.getNormal();
				glm::vec3 inwardVector = pol->getGeometricCenter()-
										 pmesh->getVertices()[firstVertexID].getCoords();
				if(glm::dot(currentFaceNormal,inwardVector)>0){
					PolygonUtils::invertVerticesOrder(pmesh, &current);
				}
			}
		}
	}
}

#endif // FIXSURFACEPOLYGONSVERTICESORDERWORKER_H
