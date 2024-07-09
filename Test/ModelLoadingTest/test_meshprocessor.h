#ifndef TEST_MESHPROCESSOR_H
#define TEST_MESHPROCESSOR_H
#include <QTest>

class UnitCube;
class MeshProcessor;
class MeshProcessorTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void vertexPolygonRelationship();
		void polygonPolygonRelationship();
		void normalPolygons();
		void normalVertices();
		void polyhedronGeocenter();
		void fixNormals();
		void cleanupTestCase();

	private:
		UnitCube* Cube;
		MeshProcessor* Tested;
	};
#endif