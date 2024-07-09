#include <QtTest>
#include "RModelTest/test_rmodel.h"
#include "ModelLoadingTest/test_modelloadingply.h"
#include "ModelLoadingTest/test_modelloadingelenode.h"
#include "ModelLoadingTest/bench_modelloadingelenode.h"
#include "ModelLoadingTest/test_modelloadingvisf.h"
#include "ModelLoadingTest/test_modelloadingoff.h"
#include "ModelLoadingTest/test_modelloadingtri.h"
#include "ModelLoadingTest/test_modelloadingm3d.h"
#include "ModelExportTest/test_modelexportvisf.h"
#include "ModelExportTest/test_modelexportelenode.h"
#include "ModelExportTest/test_modelexportoff.h"
#include "PropertyFieldLoadingTest/test_propertyfieldloadingply.h"
#include "PropertyFieldLoadingTest/test_propertyfieldloadingelenode.h"
#include "SelectionTest/test_selection.h"
#include "SelectionTest/test_selectbyid.h"
#include "SelectionTest/test_selectbyproperty.h"
#include "SelectionTest/test_changeselectiontype.h"
#include "SelectionTest/test_expandtoneighbors.h"
#include "ModelLoadingTest/bench_meshprocessor.h"
#include "ModelLoadingTest/test_meshprocessor.h"
#include <string>
#include <algorithm>

bool cmdOptionExist(char** begin, char** end, const std::string& option){
	return std::find(begin, end, option) != end;
	}

int runModelLoadingUnitTests(){
	ModelLoadingPLYTest PLY;
	ModelLoadingEleNodeTest EleNode;
	ModelLoadingVisFTest VisF;
	ModelLoadingOFFTest OFF;
	ModelLoadingTriTest TRI;
	ModelLoadingM3DTest M3D;
	MeshProcessorTest Workers;

	return QTest::qExec(&EleNode)
			| QTest::qExec(&PLY)
			| QTest::qExec(&VisF)
			| QTest::qExec(&OFF)
			| QTest::qExec(&TRI)
			| QTest::qExec(&M3D)
			| QTest::qExec(&Workers);
	}

int runModelExportUnitTests(int argc, char** argv){
	ModelExportEleNodeTest EleNode;
	ModelExportVisFTest VisF;
	ModelExportOFFTest OFF;

	bool keep = cmdOptionExist(argv, argv+argc, "-k");
	EleNode.keepTemporaryFiles = keep;
	VisF.keepTemporaryFiles = keep;
	OFF.keepTemporaryFiles = keep;

	return QTest::qExec(&EleNode)
			| QTest::qExec(&VisF)
			| QTest::qExec(&OFF);
	}

int runPropertyFieldLoadingUnitTests(){
	PropertyFieldLoadingPlyTest PLY;
	PropertyFieldLoadingEleNodeTest EleNode;

	return QTest::qExec(&PLY)
			| QTest::qExec(&EleNode);
	}

int runRenderingUnitTests(){
	RModelTest RModel;

	return QTest::qExec(&RModel);
	}

int runSelectionUnitTests(){
	SelectionTest Selection;
	SelectByIdTest StrategyById;
	SelectByPropertyTest StrategyByProperty;
	ChangeSelectionTypeTest ChangeSelection;
	ExpandToNeighborsTest ExpandSelection;

	return QTest::qExec(&Selection)
			| QTest::qExec(&StrategyById)
			| QTest::qExec(&StrategyByProperty)
			| QTest::qExec(&ChangeSelection)
			| QTest::qExec(&ExpandSelection);
	}

int runBenchmarks(){
	ModelLoadingEleNodeBench Node;
	MeshProcessorBench Workers;

	return QTest::qExec(&Node)
			| QTest::qExec(&Workers);
	}

int main(int argc, char** argv) {
	/***
	*  @note A QApplication will not execute if it finds console parameters it
	*  doesn't know, if we want to use our own, we must feed a separate argc argv
	*  parameters to it independent of the ones passed normally
	*/
	int qt_argc = 1;
	char* qt_argv[1];
	qt_argv[0] = const_cast<char*>("test_exe");
	
	// Maximum ammount of time allowed for each test case
	qputenv("QTEST_FUNCTION_TIMEOUT", "1000000");
	QApplication app(qt_argc, qt_argv);
	int status = 0;
	status |= runModelLoadingUnitTests();
	status |= runModelExportUnitTests(argc, argv);
	status |= runRenderingUnitTests();
	status |= runPropertyFieldLoadingUnitTests();
	status |= runSelectionUnitTests();

	if (cmdOptionExist(argv, argv+argc, "-B")){
		status |= runBenchmarks();
		}

	return status;
	}
