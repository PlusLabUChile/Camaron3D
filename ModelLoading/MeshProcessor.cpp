#include "ModelLoading/MeshProcessor.h"
#include "Model/Model.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "ModelLoading/ModelLoadingWorkers/polygonpolygonneighborhoodworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatepolygonnormalsworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatevertexnormalsworker.h"
#include "ModelLoading/ModelLoadingWorkers/calculatepolyhedrongeocenterworker.h"
#include "ModelLoading/ModelLoadingWorkers/fixsurfacepolygonsverticesorderworker.h"
#include "ModelLoading/ModelLoadingWorkers/vertexpolygonworker.h"
#include "Utils/qtutils.h"
#include <iostream>
MeshProcessor::MeshProcessor(){}

MeshProcessor::~MeshProcessor(){}

/***
*  @note Sequential algorithms, adding naive concurrency will cause a segfault.
*  In both steps we are pushing pointers to an element vector, which could be
*  modified by another thread at same instant. Thread safety must be ensured
*  when attempting to parallelize this steps.
*/
void MeshProcessor::completeVertexPolygonRelations(Model* pmesh, int nThreads){
	Q_UNUSED(nThreads);
	ElementsRelations* relations = pmesh->getElementsRelations();
	relations->resizePolygonsVertex(pmesh->getVerticesCount());
	for(vis::Polygon& polygon : pmesh->getPolygons()) {
		std::vector<unsigned int>& vertices = relations->getVertexPolygonsById(polygon.getId());
		for(unsigned int index_vertex: vertices){
			relations->addPolygonInVertex(index_vertex, polygon.getId());
		}
	}
}

void MeshProcessor::completePolygonPolyhedronRelations(Model* m, int nThreads){
	Q_UNUSED(nThreads);

	ElementsRelations* relations = m->getElementsRelations();
	relations->getPolyhedronPolygons().resize(m->getPolygonsCount());

	for(vis::Polyhedron& polyhedron : m->getPolyhedrons()){
		for(unsigned int index_polygon : relations->getPolygonsPolyhedronsById(polyhedron.getId())){
			relations->addPolyhedronInPolygon(polyhedron.getId(), index_polygon);
		}
	}
}
///

/***
*  @note Parallel steps, will default to a single thread process if the number of
*  elements to process is less than 10000, Details of each algorithm are located
*  in their respective worker definition.
*/

/// needs vertex polygon relations
void MeshProcessor::completePolygonPolygonRelations(Model* pmesh, int nThreads){
	if(nThreads<=1 || pmesh->getPolygonsCount()<10000)
		nThreads = 1;

	int nPolygons = pmesh->getPolygonsCount();
	pmesh->getElementsRelations()->getPolygonsPolygons().resize(nPolygons);
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		workers.push_back(new PolygonPolygonNeighborhoodWorker(pmesh,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);

}

void MeshProcessor::calculateNormalsPolygons(Model* m, int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	int nPolygons = m->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		workers.push_back(new CalculatePolygonNormalsWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);

}

void MeshProcessor::calculateNormalsVertices(Model* m, int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	int nvertices = m->getVerticesCount();
	int nBlockSize = nvertices/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nvertices;
		workers.push_back(new CalculateVertexNormalsWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}
void MeshProcessor::calculateGeoCenterPolyhedronMesh(Model* m,
															int nThreads){
	if(nThreads<=1 || m->getPolyhedronsCount()<10000)
		nThreads = 1;
	int nPolyhedrons = m->getPolyhedronsCount();
	int nBlockSize = nPolyhedrons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolyhedrons;
		workers.push_back(new CalculatePolyhedronGeocenterWorker(m,from,to));
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}
//needs geocenter and normals calculated

///@note This could come in handy when attempting to correct normal vectors on
//		 demand (after processing). Keep this here for now but remove it
//		 if said functionality doesn't need it
void MeshProcessor::fixSurfacePolygonsVerticesOrder(Model* m,
														   int nThreads){
	if(nThreads<=1 || m->getPolygonsCount()<10000)
		nThreads = 1;
	//THREADED
	int nPolygons = m->getPolygonsCount();
	int nBlockSize = nPolygons/nThreads;
	int from = 0;
	int to = nBlockSize;
	std::vector<QtWorkerBase*> workers;
	for(int i = 0;i<nThreads;i++){
		if(i == nThreads-1)//last round
			to = nPolygons;
		QtWorkerBase* worker = new FixSurfacePolygonsVerticesOrderWorker<vis::Polygon,
				vis::Polyhedron>(m,from,to);
		workers.push_back(worker);
		from += nBlockSize;
		to += nBlockSize;
	}
	QtUtils::runWorkersInThread(workers,true);
}
