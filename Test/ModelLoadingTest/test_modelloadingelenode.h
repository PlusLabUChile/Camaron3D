#ifndef TEST_MODELLOADINGELENODE_H
#define TEST_MODELLOADINGELENODE_H
#include <QTest>

class UnitCube;
class ModelLoadingEleNode;
class ModelLoadingEleNodeTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadPolygons();
		void loadPolygonsWithAttr();
		void loadPolygonsWithMarkers();
		void loadPolyhedrons();
		void loadPolyhedronsWithAttr();
		void loadPolyhedronsWithMarkers();
		void cleanupTestCase();
	private:
		ModelLoadingEleNode* Loader;
		UnitCube* sampleModel;
		std::string path = "./Test/ModelLoadingTest/data/EleNode/";
	};
#endif