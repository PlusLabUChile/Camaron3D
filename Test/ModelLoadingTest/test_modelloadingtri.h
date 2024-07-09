#ifndef TEST_MODELLOADINGTRI
#define TEST_MODELLOADINGTRI
#include <QTest>

class ModelLoadingTri;
class UnitCube;
class ModelLoadingTriTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadPolygonMesh();
		void cleanupTestCase();
	private:
		ModelLoadingTri* Loader;
		UnitCube* sampleModel;
		std::string path = "./Test/ModelLoadingTest/data/TRI/";
	};

#endif