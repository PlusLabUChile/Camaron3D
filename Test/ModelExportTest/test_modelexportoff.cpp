#include "test_modelexportoff.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "ModelLoading/ModelLoadingOff.h"
#include "ModelExport/modelexportoff.h"
#include "SelectionStrategies/Selection.h"
#include "Rendering/RModel/rmodel.h"
#include "Test/Utils/openglcontext.h"
#include <cstdio>	//std::remove

void ModelExportOFFTest::initTestCase(){
	Loader = new ModelLoadingOff();
	Tested = new ModelExportOFF();
	sampleModel = new UnitCube(false);
	}

void ModelExportOFFTest::exportVertexCloud(){
	QSKIP("Unimplemented feature");
	}

void ModelExportOFFTest::exportPolygonMesh(){
	std::string test_file = temp_folder + "polygon_mesh.off";
	Tested->exportModel(sampleModel->polygonmesh, test_file);
	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polygonmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportOFFTest::exportPolyhedronMesh(){
	/***
	* @note The OFF extension does not support polyhedrons, despite this the exporter
	* can export PolyhedronMeshes by representing them through their component polygons
	* and vertices. To test this, we export a PolyhedronMesh version of the Unit cube
	* and compare it with their PolygonMesh representation, which "should" be compatible.
	*/
	std::string test_file = temp_folder + "polyhedron_mesh.off";
	Tested->exportModel(sampleModel->polyhedronmesh, test_file);
	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polygonmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportOFFTest::exportSelection(){
	/***
	* @note Selection objects will attempt to update the current rmodel when
	* adding elements to it. This means we need an empty RModel instance for
	* this tests to work. At the same time RModel uses OpenGL specific methods,
	* which is why we need to start an OpenGL context here.
	*/
	Selection box = Selection();
	OpenGLContext context = OpenGLContext(20,20);
	try {
		context.setupContext();
		context.setupOpenGL();
		}
	catch (std::runtime_error &error){
		QFAIL(error.what());
		}

	RModel testRModel = RModel();
	// Inject elements in the Selection object
	box.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	box.setRModel(&testRModel);
	vis::Polyhedron* cube = &sampleModel->polyhedronmesh->getPolyhedrons()[0];
	box.addSelectedElement(cube);

	/// Start the test properly
	std::string test_file = temp_folder + "selection.off";
	Tested->exportSelection(&box, test_file);

	Model* result = Loader->load(test_file);
	QEXPECT_FAIL("", "Theres a discrepancy between the elements ordering in the written files, returning a different model if loaded back normally", Continue);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polygonmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportOFFTest::cleanupTestCase(){
	delete Loader;
	delete Tested;
	delete sampleModel;
	}