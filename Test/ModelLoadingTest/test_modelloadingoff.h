#ifndef TEST_MODELLOADINGOFF
#define TEST_MODELLOADINGOFF
#include <QTest>

class ModelLoadingOff;
class UnitCube;
class ModelLoadingOFFTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadPolygonMesh();
		void loadVertexCloud();
		void cleanupTestCase();
	private:
		ModelLoadingOff* Loader;
		UnitCube* sampleModel;
		std::string path = "./Test/ModelLoadingTest/data/OFF/";
	};

#endif