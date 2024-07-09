#ifndef TEST_MODELLOADING_H
#define TEST_MODELLOADING_H
#include <QTest>

class ModelLoadingVisF;
class UnitCube;
class ModelLoadingVisFTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadVertexCloud();
		void loadPolygons();
		void loadPolyhedrons();
		void loadBigEndian();
		void loadASCII();
		void cleanupTestCase();
	private:
		ModelLoadingVisF* Loader;
		UnitCube* sampleModel;
		std::string path = "./Test/ModelLoadingTest/data/VisF/";
	};
#endif