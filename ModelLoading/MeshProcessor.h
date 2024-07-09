#ifndef MESH_PROCESSOR_H
#define MESH_PROCESSOR_H

#include <string>
#include <vector>
#include <QThread>
#define N_DEFAULT_THREADS_MODEL_LOADING 8

class Model;
class MeshProcessor {
	public:
		MeshProcessor();
		~MeshProcessor();

		static void completeVertexPolygonRelations(Model* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void completePolygonPolygonRelations(Model* pmesh,
						int nThreads = N_DEFAULT_THREADS_MODEL_LOADING); //needs vertex Polygons relations
		static void completePolygonPolyhedronRelations(Model* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateNormalsVertices(Model* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateNormalsPolygons(Model* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
		static void calculateGeoCenterPolyhedronMesh(Model* m, int nThread = N_DEFAULT_THREADS_MODEL_LOADING);


		///@deprecated
		static void fixSurfacePolygonsVerticesOrder(Model* m, int nThreads = N_DEFAULT_THREADS_MODEL_LOADING);
	};

#endif // MESH_PROCESSOR_H
