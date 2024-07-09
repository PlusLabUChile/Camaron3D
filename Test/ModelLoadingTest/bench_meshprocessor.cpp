#include "bench_meshprocessor.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "ModelLoading/modelloadingelenode.h"
#include "Test/Utils/Mocks/MockModelLoading.h"
#include "ModelLoading/MeshProcessor.h"

void MeshProcessorBench::initTestCase(){
	Mock = new MockModelLoading();
	Loader = new ModelLoadingEleNode();

	std::cout << "Loading test file...";
	stressModel = Loader->load("./Test/ModelLoadingTest/benchmark/gen262144.1.ele"); //Sphere 7
	delete Loader;
	std::cout << "\t[DONE]" << std::endl;
	}

/// Stress testing
void MeshProcessorBench::stressVertexPolygonRelationship(){
	compareImplementations(&MockModelLoading::SEQcompleteVertexPolygonRelations,
						   &MeshProcessor::completeVertexPolygonRelations,
						   &MeshProcessorBench::resetVertexPolygonRelationship);

	}

void MeshProcessorBench::stressPolygonPolyhedronRelationship(){
	compareImplementations(&MockModelLoading::SEQcompletePolygonPolyhedronRelations,
						   &MeshProcessor::completePolygonPolyhedronRelations,
						   &MeshProcessorBench::resetPolygonPolyhedronRelationship);

	}

void MeshProcessorBench::stressNormalPolygons(){
	// Normal calculations are independent of previous stages, no need for cleanup procedure
	compareImplementations(&MockModelLoading::SEQcalculateNormalsPolygons,
						   &MeshProcessor::calculateNormalsPolygons);
	}

void MeshProcessorBench::stressPolyhedronGeocenter(){
	// GeoCenter calculations will override the previous value, no need for cleanup procedure
	compareImplementations(&MockModelLoading::SEQcalculateGeoCenterPolyhedronMesh,
						   &MeshProcessor::calculateGeoCenterPolyhedronMesh);

	}

void MeshProcessorBench::stressPolygonPolygonRelationship(){
	// This method requires that vertex-polygon relationships are already computed
	std::cout << "Populating VPR...";
	MeshProcessor::completeVertexPolygonRelations(stressModel);
	std::cout <<"[DONE]" << std::endl;
	//

	compareImplementations(&MockModelLoading::SEQcompletePolygonPolygonRelations,
						   &MeshProcessor::completePolygonPolygonRelations,
						   &MeshProcessorBench::resetPolygonPolygonRelationship);

	resetVertexPolygonRelationship(stressModel);
	}

void MeshProcessorBench::stressNormalVertices(){
	// This method requires that vertex-polygon relations, polygon normals and polygon-polyhedron relationships (In the case of Polyhedron meshes)
	std::cout << "Populating VPR...";
	MeshProcessor::completeVertexPolygonRelations(stressModel);
	std::cout <<"[DONE]" << std::endl;

	std::cout << "Populating PPHR...";
	MeshProcessor::completePolygonPolyhedronRelations(stressModel);
	std::cout <<"[DONE]" << std::endl;

	std::cout << "Populating PN...";
	MeshProcessor::calculateNormalsPolygons(stressModel);
	std::cout <<"[DONE]" << std::endl;
	///

	// Normal computation overrides previous values, no need for cleanup

	compareImplementations(&MockModelLoading::SEQcalculateNormalsVertices,
						   &MeshProcessor::calculateNormalsVertices);

	resetVertexPolygonRelationship(stressModel);
	resetPolygonPolyhedronRelationship(stressModel);
	}

/// Cleanup methods, undo the changes made by their respective methods
void MeshProcessorBench::resetVertexPolygonRelationship(Model* pmesh){
	for(vis::Vertex& vertex : pmesh->getVertices()){
		std::vector<vis::Polygon*>& neighbors = vertex.getVertexPolygons();
		neighbors.clear();
		std::vector<vis::Polygon*>().swap(neighbors);
		}
	}

void MeshProcessorBench::resetPolygonPolygonRelationship(Model* pmesh){
	for(vis::Polygon& polygon : pmesh->getPolygons()){
		std::vector<vis::Polygon*>& neighbors = polygon.getNeighborPolygons();
		neighbors.clear();
		std::vector<vis::Polygon*>().swap(neighbors);
		}
	}

void MeshProcessorBench::resetPolygonPolyhedronRelationship(Model* pmesh){
	for(vis::Polygon& polygon : pmesh->getPolygons()){
		vis::Polyhedron** polyhedrons = polygon.getNeighborPolyhedron();
		polyhedrons[0] = (vis::Polyhedron*)0;
		polyhedrons[1] = (vis::Polyhedron*)0;
		}
	}

/***
* @brief Compares execution time between a parallel method and its sequential equivalent
*
* The method will compare the sequential execution and the parallel version for the default
* number of threads specified in ModelLoadingStrategyBench
*
*/
void MeshProcessorBench::compareImplementations(void(MockModelLoading::*sequential)(Model*),
													   void(threaded)(Model*, int),
													   void(MeshProcessorBench::*cleanup)(Model*)){
	for (int i = 0; i < ITERATION_NUMBER; i++){
		CrossTimer timerSeq;
		(Mock->*sequential)(stressModel);
		std::cout << "Sequential Time: " << timerSeq.getTranscurredMilliseconds() << std::endl;

		(this->*cleanup)(stressModel);

		CrossTimer timerThre;
		threaded(stressModel, DEFAULT_THREAD_NUMBER);
		std::cout << "Threaded Time: " << timerThre.getTranscurredMilliseconds() << std::endl;

		(this->*cleanup)(stressModel);
		}
	}

void MeshProcessorBench::compareImplementations(void(MockModelLoading::*sequential)(Model*),
													   void(threaded)(Model*, int)){

	for (int i = 0; i <ITERATION_NUMBER; i++){
		CrossTimer timerSeq;
		(Mock->*sequential)(stressModel);
		std::cout << "Sequential Time: " << timerSeq.getTranscurredMilliseconds() << std::endl;

		CrossTimer timerThre;
		threaded(stressModel, DEFAULT_THREAD_NUMBER);
		std::cout << "Threaded Time: " << timerThre.getTranscurredMilliseconds() << std::endl;
		}
	}

void MeshProcessorBench::cleanupTestCase(){
	delete Mock;
	delete stressModel;
	}
