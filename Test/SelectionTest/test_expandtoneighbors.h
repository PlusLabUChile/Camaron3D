#ifndef TEST_EXPANDTONEIGHBORS_H
#define TEST_EXPANDTONEIGHBORS_H

#include <QTest>
#include <QWidget>

class RModel;
class Model;
class UnitCube;
class ExpandToNeighbors;
class ExpandToNeighborsConfig;
class ExpandToNeighborsTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void init();
		void expandAllPolygons();
		void expandAllPolyhedrons();
		void expandSurfacePolygons();
		void expandSurfacePolyhedrons();
		void expandSurfaceWithAnglePolygons();
		void expandSurfaceWithAnglePolyhedrons();
		void cleanup();
		void cleanupTestCase();

	private:
		RModel* rmodel;
		UnitCube* cube;
		Model* test_model;
		ExpandToNeighbors* tested;
		ExpandToNeighborsConfig* config;
	};

#endif