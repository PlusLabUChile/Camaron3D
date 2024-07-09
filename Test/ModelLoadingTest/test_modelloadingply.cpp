#include "test_modelloadingply.h"
#include "ModelLoading/modelloadingply.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Exceptions/modelloadingexception.h"
#include "Model/Model.h"

void ModelLoadingPLYTest::initTestCase(){
	Loader = new ModelLoadingPly();
	sampleModel = new UnitCube(false);
	}

void ModelLoadingPLYTest::validationTest(){
	/**
	 * Validation testing, method should return true if the header contains correct
	 * information, false otherwise. Data included in the body of the file will not
	 * be tested, as the load method should handle said edge case
	 */
	QVERIFY(Loader->validate(path + "ascii.ply"));
	QCOMPARE(Loader->validate(path + "data.off"), false);
	QCOMPARE(Loader->validate(path + "non_existent.ply"), false);

	// If a file header does not contain the "end_header" keyword, it should return false
	// to prevent the program reaching an Invalid State
	QEXPECT_FAIL("","Current version reads past the header until reaching the EOF char, returning true", Continue);
	QCOMPARE(Loader->validate(path + "no_end_header.ply"), false);
	}

void ModelLoadingPLYTest::loadASCII(){
	Model* ModelBuffer = Loader->load(path + "ascii.ply");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
}

void ModelLoadingPLYTest::loadLittleEndian(){
	Model* ModelBuffer = Loader->load(path + "binary_little.ply");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingPLYTest::loadBigEndian(){
	Model* ModelBuffer = Loader->load(path + "binary_big.ply");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingPLYTest::loadVertexCloud(){
	Model* ModelBuffer = Loader->load(path + "vertex_cloud.ply");
	QVERIFY(ModelBuffer != nullptr);
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->vertexcloud));
	}

void ModelLoadingPLYTest::loadDifferentTypes(){
	/** Regression Test
	 * The following binary file is failing due to it containing non-float properties,
	 * this means that when attempting to fetch data from each vertex, the loader
	 * caret will miscalculate the position of the next vertex
	 *
	 * @todo Include types with different types
	 * @todo Include face properties
	 */
	Model* ModelBuffer = Loader->load(path + "other_property_types.ply");
	QVERIFY(ModelBuffer != nullptr);

	//Model should be still usable
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->polygonmesh));
	delete ModelBuffer;
	}

void ModelLoadingPLYTest::loadAdditionalEdges(){
	Model* ModelBuffer = Loader->load(path + "additional_edges.ply");
	QVERIFY(ModelBuffer != nullptr);
	sampleModel->setAdditionalEdges();
	QVERIFY(MeshExtensions::compare(ModelBuffer, sampleModel->vertexcloud));
	delete ModelBuffer;
	}

void ModelLoadingPLYTest::loadBadHeader(){
	// Edge case of header without an end_header tag should be caught by a validation check,
	// if the file is loaded without said validation, we should throw an exception anyway.
	Model* ModelBuffer;
	QVERIFY_EXCEPTION_THROWN(ModelBuffer = Loader->load(path + "no_end_header.ply"), ModelLoadingException);

	// Model header requests more elements than what is actually contained
	QVERIFY_EXCEPTION_THROWN(ModelBuffer = Loader->load(path + "wrong_poly_count.ply"), ModelLoadingException);

	Q_UNUSED(ModelBuffer);
	}

void ModelLoadingPLYTest::cleanupTestCase(){
	delete Loader;
	delete sampleModel;
	}
