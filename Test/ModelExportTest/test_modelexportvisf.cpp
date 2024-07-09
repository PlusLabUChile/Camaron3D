#include "test_modelexportvisf.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "ModelLoading/modelloadingvisf.h"
#include "ModelExport/modelexportvisf.h"
#include "SelectionStrategies/Selection.h"
#include "Rendering/RModel/rmodel.h"
#include "Test/Utils/openglcontext.h"
#include <cstdio> //std::remove

void ModelExportVisFTest::initTestCase(){
	Loader = new ModelLoadingVisF();
	Tested = new ModelExportVisF();
	sampleModel = new UnitCube(false);
	}

void ModelExportVisFTest::exportVertexCloud(){
	/***
	 * @todo While this verifies that the content was saved properly, it does
	 * depend on the ModelLoading implementation and the os file system.
	 * See if you can verify the content in a more independent manner (inspecting
	 * the stream for example)
	 */
	std::string test_file = temp_folder + "vertex_cloud.visf";
	Tested->exportModel(sampleModel->vertexcloud, test_file);
	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->vertexcloud));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportVisFTest::exportPolygonMesh(){
	std::string test_file = temp_folder + "polygon_mesh.visf";
	Tested->exportModel(sampleModel->polygonmesh, test_file);
	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polygonmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportVisFTest::exportPolyhedronMesh(){
	std::string test_file = temp_folder + "polyhedron_mesh.visf";
	Tested->exportModel(sampleModel->polyhedronmesh, test_file);
	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polyhedronmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportVisFTest::exportSelection(){
	// @see ModelExportOffTest for more details about this setup
	QFAIL("Current version generates an OFF file instead of a VisF one");
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
	std::string test_file = temp_folder + "selection.visf";
	Tested->exportSelection(&box, test_file);

	Model* result = Loader->load(test_file);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polyhedronmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_file.c_str());
		}
	}

void ModelExportVisFTest::cleanupTestCase(){
	delete Loader;
	delete Tested;
	delete sampleModel;
	}