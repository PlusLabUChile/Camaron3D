#include "test_modelexportelenode.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/meshextensions.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "ModelLoading/modelloadingelenode.h"
#include "ModelExport/modelexportelenode.h"
#include "SelectionStrategies/Selection.h"

void ModelExportEleNodeTest::initTestCase(){
	Loader = new ModelLoadingEleNode();
	Tested = new ModelExportEleNode();
	sampleModel = new UnitCube(true);
	}
void ModelExportEleNodeTest::exportVertexCloud(){
	QSKIP("Unimplemented feature");
	}
void ModelExportEleNodeTest::exportPolygonMesh(){
	QSKIP("TRIANGLE only represents 2D surfaces, we cannot use a cube model here");
	std::string test_ele_file = temp_folder + "polygon_mesh.ele";
	std::string test_node_file = temp_folder + "polygon_mesh.node";
	Tested->exportModel(sampleModel->polygonmesh, test_ele_file);
	Model* result = Loader->load(test_ele_file);
	QEXPECT_FAIL("","Current version enforces an ID number sequence, independent of the model contents", Continue);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polygonmesh));
	delete result;
	if (!keepTemporaryFiles){
		std::remove(test_ele_file.c_str());
		std::remove(test_node_file.c_str());
		}
	}

void ModelExportEleNodeTest::exportPolyhedronMesh(){
	std::string test_ele_file = temp_folder + "polyhedron_mesh.ele";
	std::string test_node_file = temp_folder + "polyhedron_mesh.node";
	Tested->exportModel(sampleModel->polyhedronmesh, test_ele_file);
	Model* result = Loader->load(test_ele_file);
	QEXPECT_FAIL("","There's a discrepancy between the order of vertex IDs in the model between load and export operations. They should be reversable operations.", Continue);
	QVERIFY(MeshExtensions::compare(result, sampleModel->polyhedronmesh));
	delete result;

	if (!keepTemporaryFiles){
		std::remove(test_ele_file.c_str());
		std::remove(test_node_file.c_str());
		}
	}
void ModelExportEleNodeTest::exportSelection(){
	QSKIP("Unimplemented feature");
	}

void ModelExportEleNodeTest::cleanupTestCase(){
	delete Loader;
	delete Tested;
	delete sampleModel;
	}