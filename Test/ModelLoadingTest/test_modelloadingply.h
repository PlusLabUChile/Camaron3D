#ifndef TEST_MODELLOADINGPLY
#define TEST_MODELLOADINGPLY
#include <QTest>

class ModelLoadingPly;
class UnitCube;
class ModelLoadingPLYTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void validationTest();
		void loadASCII();
		void loadLittleEndian();
		void loadBigEndian();
		void loadVertexCloud();
		void loadDifferentTypes();
		void loadAdditionalEdges();
		void loadBadHeader();
		void cleanupTestCase();
	private:
		ModelLoadingPly* Loader;
		UnitCube* sampleModel;
		std::string path = "./Test/ModelLoadingTest/data/PLY/";
	};

#endif