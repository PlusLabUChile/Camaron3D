#ifndef TEST_MODELEXPORTELENODE_H
#define TEST_MODELEXPORTELENODE_H
#include <QTest>

class ModelExportEleNode;
class ModelLoadingEleNode;
class UnitCube;
class ModelExportEleNodeTest: public QObject{
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
		ModelExportEleNode* Tested;
		ModelLoadingEleNode* Loader;	//Read generated files
		UnitCube* sampleModel;
		std::string temp_folder = "./Test/ModelExportTest/temp/";

};

#endif // TEST_MODELEXPORTELENODE_H