#include "calculatepolyhedrongeocenterworker.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "Utils/polyhedronutils.h"

CalculatePolyhedronGeocenterWorker::CalculatePolyhedronGeocenterWorker(Model* mesh,
																	   unsigned int min,
																	   unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Compute the geometric center coordinates for a range of polyhedrons
*
* Data computed here will be used later by the classes listed below:
*
* @see Polyhedron for volume and normal (inward or outward) calculations
* @see ConvexGeometryIntersectionRenderer
* @see Renderer::isDisplayedViewPortCheck
* @see Fixsurfacepolygonsverticesorderworker
* @see PolyhedronUtils::getPolyhedronSolidAngleFromVertex
*
* @todo Leaving this information to be computed on demand doesn't seem to reduce
* performance significantly. Need to benchmark the list above to be sure but if
* no bottlenecks are generated, then remove this step from the pipeline and remove
* the geocenter field from the Polyhedron instances.
*
*/
void CalculatePolyhedronGeocenterWorker::doWork(){
	std::vector<vis::Polyhedron>& polyhedrons = pmesh->getPolyhedrons();
	for(unsigned int i = rangeMin; i < rangeMax; i++){
		PolyhedronUtils::calculateGeometricCenter(pmesh, &polyhedrons[i]);
	}
}
