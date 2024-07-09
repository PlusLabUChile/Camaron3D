#include "test_modelloadingelenode.h"
#include "Model/Model.h"
#include "ModelLoading/modelloadingelenode.h"
#include "Test/Utils/meshextensions.h"
#include "Test/Utils/unitcube.h"
#include <QString>

void ModelLoadingEleNodeTest::initTestCase(){
	Loader = new ModelLoadingEleNode();
	sampleModel = new UnitCube(true);
	}

void ModelLoadingEleNodeTest::validationTest(){
	/**
	 * Validation testing, current version only checks if there is a complementary file
	 * to the given filename, otherwise returns false
	 */

	QVERIFY(Loader->validate(path + "polygon_mesh.ele"));
	QVERIFY(Loader->validate(path + "polygon_mesh.node"));

	QCOMPARE(Loader->validate(path + "data.off"), false);
	QCOMPARE(Loader->validate(path + "orphan.ele"), false);
	}


void ModelLoadingEleNodeTest::loadPolygons(){
	QSKIP("TRIANGLE only represents 2D surfaces, we cannot use a cube model here");
	Model* ModelBuffer = Loader->load(path + "polygon_mesh.ele");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingEleNodeTest::loadPolygonsWithAttr(){
	QSKIP("Unimplemented, not needed right now but might be useful in the future");
	}

void ModelLoadingEleNodeTest::loadPolygonsWithMarkers(){
	QSKIP("Unimplemented, not needed right now but might be useful in the future");
	}
void ModelLoadingEleNodeTest::loadPolyhedrons(){
	Model* ModelBuffer = Loader->load(path + "polyhedron_mesh.ele");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polyhedronmesh));
	delete ModelBuffer;
	}

void ModelLoadingEleNodeTest::loadPolyhedronsWithAttr(){
	QSKIP("Unimplemented, not needed right now but might be useful in the future");
	}

void ModelLoadingEleNodeTest::loadPolyhedronsWithMarkers(){
	QSKIP("Unimplemented, not needed right now but might be useful in the future");
	}

void ModelLoadingEleNodeTest::cleanupTestCase(){
	delete Loader;
	delete sampleModel;
	}