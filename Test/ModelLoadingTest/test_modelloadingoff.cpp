#include "test_modelloadingoff.h"
#include "ModelLoading/modelloadingOff.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Model.h"

void ModelLoadingOFFTest::initTestCase(){
	Loader = new ModelLoadingOff();
	sampleModel = new UnitCube(false);
	}

void ModelLoadingOFFTest::validationTest(){
	/**
	 * Validation only checks if the model has the OFF string at the beginning
	 */
	QVERIFY(Loader->validate(path + "polygon_mesh.off"));
	QCOMPARE(Loader->validate(path + "model.ele"), false);
	QCOMPARE(Loader->validate(path + "non_existent.off"), false);
	}

void ModelLoadingOFFTest::loadPolygonMesh(){
	Model* ModelBuffer = Loader->load(path + "polygon_mesh.off");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
}

void ModelLoadingOFFTest::loadVertexCloud(){
	Model* ModelBuffer = Loader->load(path + "vertex_cloud.off");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->vertexcloud));
	}

void ModelLoadingOFFTest::cleanupTestCase(){
	delete Loader;
	delete sampleModel;
	}
