#include "bench_modelloadingelenode.h"
#include <QString>
#include "Model/Model.h"


void ModelLoadingEleNodeBench::initTestCase(){
	Loader = new ModelLoadingEleNode();
	}

void ModelLoadingEleNodeBench::sphereModelLoad_data(){
	QTest::addColumn<QString>("path");

	QString folder = "./Test/ModelLoadingTest/benchmark/";

	QTest::newRow("Sphere 1") << (folder + "gen4096.1.ele");
	QTest::newRow("Sphere 2") << (folder + "gen8192.1.ele");
	QTest::newRow("Sphere 3") << (folder + "gen16384.1.ele");
	QTest::newRow("Sphere 4") << (folder + "gen32768.1.ele");
	QTest::newRow("Sphere 5") << (folder + "gen65536.1.ele");
	QTest::newRow("Sphere 6") << (folder + "gen131072.1.ele");
	QTest::newRow("Sphere 7") << (folder + "gen262144.1.ele");
	//QTest::newRow("Sphere 8") << (folder + "gen524288.1.ele"); // 70 secs

	// Thrashing limit, performance improvements will move this limit downwards

	//QTest::newRow("Sphere 9") << (folder + "gen1048576.1.ele"); // 5100 secs
	//QTest::newRow("Sphere 10") << (folder + "gen2097152.1.ele");
	//QTest::newRow("Sphere 11") << (folder + "gen4194304.1.ele");
	//QTest::newRow("Sphere 12") << (folder + "gen4194304.1.ele");
	}

void ModelLoadingEleNodeBench::sphereModelLoad(){
	QFETCH(QString, path);
	QBENCHMARK{
		Model* ModelBuffer = Loader->load(path.toStdString());
		delete ModelBuffer;
		}
	}

void ModelLoadingEleNodeBench::cleanupTestCase(){
	delete Loader;
	}