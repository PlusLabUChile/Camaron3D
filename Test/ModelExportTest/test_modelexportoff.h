#ifndef TEST_MODELEXPORTOFF_H
#define TEST_MODELEXPORTOFF_H
#include <QTest>

class ModelExportOFF;
class ModelLoadingOff;
class UnitCube;
class ModelExportOFFTest: public QObject{
	Q_OBJECT

	public:
		bool keepTemporaryFiles = false;

	private slots:
		void initTestCase();
		void exportVertexCloud();
		void exportPolygonMesh();
		void exportPolyhedronMesh();
		void exportSelection();
		void cleanupTestCase();

	private:
		ModelExportOFF* Tested;
		ModelLoadingOff* Loader;	//Read generated files
		UnitCube* sampleModel;
		std::string temp_folder = "./Test/ModelExportTest/temp/";

};

#endif // TEST_MODELEXPORTELENODE_H