#ifndef TEST_MODELLOADINGM3D
#define TEST_MODELLOADINGM3D
#include <QTest>

class ModelLoadingM3D;
class UnitCube;
class ModelLoadingM3DTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadHexahedronCube();
		void loadTetrahedronCube();
		void cleanupTestCase();
	private:
		ModelLoadingM3D* Loader;
		std::string path = "./Test/ModelLoadingTest/data/M3D/";
	};

#endif