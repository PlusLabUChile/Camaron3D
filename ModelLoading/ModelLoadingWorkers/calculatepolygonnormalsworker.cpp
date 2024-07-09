#include "calculatepolygonnormalsworker.h"
#include "Model/Element/Polygon.h"
#include "Model/Model.h"
#include "Utils/PolygonUtils.h"

CalculatePolygonNormalsWorker::CalculatePolygonNormalsWorker(Model * mesh,
															 unsigned int min,
															 unsigned int max):
	QtWorkerBase()
{
	pmesh = mesh;
	rangeMin = min;
	rangeMax = max;
}

/**
* @brief Compute face normals for a range of polygons in the mesh
*
* Face normals are used in the processing pipeline when computing vertex normals
* and when correcting normal orientation (if requested). This data is later used
* by the following classes:
*
* @see PolygonUtils::getDihedralAngle
* @see Polygon::getArea
* @see MouseSelection
*
*/
void CalculatePolygonNormalsWorker::doWork(){
	std::vector<vis::Polygon>& polygons = pmesh->getPolygons();
	for(unsigned int i = rangeMin; i < rangeMax; i++){
		PolygonUtils::calculateNormal(pmesh, &polygons[i]);
	}
}
