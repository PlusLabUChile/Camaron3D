#include "test_modelloadingm3d.h"
#include "ModelLoading/modelloadingm3d.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Model.h"

void ModelLoadingM3DTest::initTestCase(){
	Loader = new ModelLoadingM3D();
	}

void ModelLoadingM3DTest::validationTest(){
	/**
	 * Validation verifies if the file starts with the string [Nodes, ARRAY1<STRING>]
	 */
	QVERIFY(Loader->validate(path + "cube.m3d"));
	QCOMPARE(Loader->validate(path + "data.off"), false);
	QCOMPARE(Loader->validate(path + "non_existent.m3d"), false);
	}

void ModelLoadingM3DTest::loadHexahedronCube(){
	UnitCube sampleModel(false);
	Model* ModelBuffer = Loader->load(path + "cube.m3d");
	QVERIFY(ModelBuffer != nullptr);
	QEXPECT_FAIL("","Element ordering is different for this format than others, will need a better test model to compare.", Continue);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel.polyhedronmesh));
	delete ModelBuffer;
}

void ModelLoadingM3DTest::loadTetrahedronCube(){
	UnitCube sampleModel(true);
	Model* ModelBuffer = Loader->load(path + "tetrahedrons.m3d");
	QVERIFY(ModelBuffer != nullptr);
	QEXPECT_FAIL("","Element ordering is different for this format than others, will need a better test model to compare.", Continue);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel.polyhedronmesh));
	delete ModelBuffer;
	}

void ModelLoadingM3DTest::cleanupTestCase(){
	delete Loader;
	}
