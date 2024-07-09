#ifndef BENCH_MESHPROCESSOR_H
#define BENCH_MESHPROCESSOR_H

#include <QTest>
#include "Utils/crosstimer.h"
#include <iostream>
#include <functional>

#define ITERATION_NUMBER 5
#define DEFAULT_THREAD_NUMBER 8
class Model;
class ModelLoadingEleNode;
class MeshProcessor;
class MockModelLoading;
class MeshProcessorBench: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();

		// Stress tests
		void stressVertexPolygonRelationship();
		void stressPolygonPolyhedronRelationship();
		void stressNormalPolygons();
		void stressPolyhedronGeocenter();
		void stressPolygonPolygonRelationship();
		void stressNormalVertices();

		void cleanupTestCase();
	private:
		MockModelLoading* Mock;
		Model* stressModel;
		ModelLoadingEleNode* Loader;

		void resetVertexPolygonRelationship(Model* mesh);
		void resetPolygonPolygonRelationship(Model* mesh);
		void resetPolygonPolyhedronRelationship(Model* mesh);

		void compareImplementations(void(MockModelLoading::*sequential)(Model*),
									void(threaded)(Model*, int),
									void(MeshProcessorBench::*cleanup)(Model*));

		void compareImplementations(void(MockModelLoading::*sequential)(Model*),
									void(threaded)(Model*, int));
	};

#endif