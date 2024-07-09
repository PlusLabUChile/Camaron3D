#ifndef TEST_MODELEXPORTVISF_H
#define TEST_MODELEXPORTVISF_H
#include <QTest>

class ModelExportVisF;
class ModelLoadingVisF;
class UnitCube;
class ModelExportVisFTest: public QObject{
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
		ModelExportVisF* Tested;
		ModelLoadingVisF* Loader;	//Read generated files
		UnitCube* sampleModel;
		std::string temp_folder = "./Test/ModelExportTest/temp/";

};

#endif // TEST_MODELEXPORTVISF_H