#include "test_modelloadingtri.h"
#include "ModelLoading/modelloadingtri.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Model.h"

void ModelLoadingTriTest::initTestCase(){
	Loader = new ModelLoadingTri();
	sampleModel = new UnitCube(true);
	}

void ModelLoadingTriTest::validationTest(){
	QVERIFY(Loader->validate(path + "polygon_mesh.tri"));
	QEXPECT_FAIL("","Currently there is no validation for this file format", Continue);
	QCOMPARE(Loader->validate(path + "data.off"), false);
	QEXPECT_FAIL("","Currently there is no validation for this file format", Continue);
	QCOMPARE(Loader->validate(path + "non_existent.tri"), false);
	}

void ModelLoadingTriTest::loadPolygonMesh(){
	Model* ModelBuffer = Loader->load(path + "polygon_mesh.tri");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
}

void ModelLoadingTriTest::cleanupTestCase(){
	delete Loader;
	delete sampleModel;
	}
