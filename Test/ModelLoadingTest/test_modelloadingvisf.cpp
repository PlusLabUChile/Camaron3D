#include "test_modelloadingvisf.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Model/Model.h"
#include "ModelLoading/modelloadingvisf.h"


void ModelLoadingVisFTest::initTestCase(){
	Loader = new ModelLoadingVisF();
	sampleModel = new UnitCube(false);
	}

void ModelLoadingVisFTest::validationTest(){
	/**
	 * Validation testing, for the current specifications, the file must have: A byte
	 * representing endianess, an int representing the mesh type.
	 *
	 * NOTE: Some older files lack the first endianess byte, which will cause a segfault if
	 *		 missing, to recover the data, use a hex editor to insert a "0x01" byte at the
	 *		 beginning of the file.
	 */

	QVERIFY(Loader->validate(path + "ascii.visf"));
	QVERIFY(Loader->validate(path + "little_endian.visf"));
	QVERIFY(Loader->validate(path + "vertex_cloud.visf"));

	// TODO: Determine course of action for older models
	// If valid => Must not crash
	// If not valid => Must detect the lack of this byte and cause a validation error
	QCOMPARE(Loader->validate(path + "old.visf"), false);
	}

void ModelLoadingVisFTest::loadVertexCloud(){
	Model* ModelBuffer = Loader->load(path + "vertex_cloud.visf");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->vertexcloud));
	delete ModelBuffer;
	}

void ModelLoadingVisFTest::loadPolygons(){
	Model* ModelBuffer = Loader->load(path + "little_endian.visf");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingVisFTest::loadPolyhedrons(){
	Model* ModelBuffer = Loader->load(path + "polyhedron_mesh.visf");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polyhedronmesh));
	delete ModelBuffer;
	}

void ModelLoadingVisFTest::loadBigEndian(){
	Model* ModelBuffer = Loader->load(path + "big_endian.visf");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingVisFTest::loadASCII(){
	Model* ModelBuffer = Loader->load(path + "ascii.visf");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polyhedronmesh));

	delete ModelBuffer;
	}

void ModelLoadingVisFTest::cleanupTestCase(){
	delete Loader;
	delete sampleModel;
	}
