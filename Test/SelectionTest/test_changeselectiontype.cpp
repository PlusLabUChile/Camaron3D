#include "test_changeselectiontype.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/openglcontext.h"
#include "Common/Constants.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "ModelLoading/MeshProcessor.h"
#include "SelectionStrategies\Selection.h"
#include "SelectionStrategies\ChangeSelectionType\changeselectiontype.h"
#include "SelectionStrategies\ChangeSelectionType\changeselectiontypeconfig.h"
#include "Rendering/RModel/rmodel.h"
#include <QRadioButton>
#include <QComboBox>

void ChangeSelectionTypeTest::initTestCase(){

	///@note Requirement for setupPreApplying tests
	context = new OpenGLContext(20, 20);
	try{
		context->setupContext();
		context->setupOpenGL();
		}
	catch (std::runtime_error &error){
		delete context;
		QFAIL(error.what());
		}
	rmodel = new RModel();
	///

	tested = new ChangeSelectionType();
	tested->QApplicationInitiated();
	config = (ChangeSelectionTypeConfig*)tested->getConfigWidget();
	}

/***
*  @brief Verify that its possible to swap between polygons and polyhedrons
*
*  Vertex transitions are not supported as of yet. Note that when using
*  setupPreApplying we are querying the type of the Model to the passed RModel
*  instance, this is due to the fact that the selection cannot create new instances
*  during the swap, which restricts our algorithm to the existing objects in the model.
*
*  @todo In theory the Selection object already has a reference to RModel, see
*  if we can reuse said reference instead of relying on passing said pointer as
*  parameter.
*
*/
void ChangeSelectionTypeTest::setupCompatibleConversion(){
	UnitCube cube(false);
	Selection sel = Selection();
	sel.setRModel(rmodel);

	//Polygon to Polyhedron within PolyhedronMesh
	Model* test_model = cube.polyhedronmesh;
	rmodel->loadRModelData(test_model);
	sel.setSelectionType(vis::CONSTANTS::POLYGON);


	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(tested->setupPreApplying(&sel, rmodel));

	//Polyhedron to Polygon within PolyhedronMesh
	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);

	modelType = config->findChild<QRadioButton*>("radio_polygon");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(tested->setupPreApplying(&sel, rmodel));
	}

/***
*  @brief Selections between incompatible models should be canceled by the program
*
*  Current incompatible selections include, vertex cloud to polygons and polyhedrons,
*  polygon mesh to any element type.
*
*  @todo While discarding use cases that do not apply is fine, we should have a
*  way to tell the user what is supported and what is not. Right now there's no
*  indication if the selection succeeded or not due to this issue.
*/
void ChangeSelectionTypeTest::setupIncompatibleConversion(){
	UnitCube cube(false);
	Selection sel = Selection();
	sel.setRModel(rmodel);

	// Vertex Cloud to polygon or polyhedron
	Model* vertex_cloud = cube.vertexcloud;
	rmodel->loadRModelData(vertex_cloud);

	sel.setSelectionType(vis::CONSTANTS::POLYGON);

	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polygon");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(!tested->setupPreApplying(&sel, rmodel));

	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(!tested->setupPreApplying(&sel, rmodel));

	// Polygon Mesh to vertex and polyhedrons
	Model* polygon_mesh = cube.polygonmesh;
	rmodel->loadRModelData(polygon_mesh);

	///@note Vertex selection is currently disabled, we will skip this check for the time being
	/*
	sel.setSelectionType(vis::CONSTANTS::VERTEX);
	modelType = config->findChild<QRadioButton*>("radio_vertex");
	modelType->setChecked(true);

	QVERIFY(!tested->setupPreApplying(&sel, rmodel);
	*/

	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(!tested->setupPreApplying(&sel, rmodel));
}

/***
*  @brief Attempting to select between the same types should return false no
*  matter the model type being used.
*
*  @note No need to setup a model or a Rmodel here, as the check is between
*  the selection type and the UI model option
*/
void ChangeSelectionTypeTest::setupSameConversion(){
	Selection sel = Selection();

	/* Same reason as above
	sel.setSelectionType(vis::CONSTANTS::VERTEX);
	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_vertex");
	modelType->setChecked(true);

	QVERIFY(!tested->setupPreApplying(&sel, (RModel*)0);
	*/

	sel.setSelectionType(vis::CONSTANTS::POLYGON);
	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polygon");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(!tested->setupPreApplying(&sel, (RModel*)0));

	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	tested->setup();
	QVERIFY(!tested->setupPreApplying(&sel, (RModel*)0));
}

/***
*  @brief Test polygon transition between elements in the case of exclusive selection
*
*  Test case starts with a tetrahedron representation of the unit cube, we select
*  all the surface faces of the cube and apply the type change. In this use case
*  the selection group should return nothing, as no tetrahedrons in the mesh will
*  have all its faces selected.
*/
void ChangeSelectionTypeTest::polygonToPolyhedronExclusive(){
	UnitCube cube(true);
	Selection sel = Selection();
	Model* test_model = cube.polyhedronmesh;
	MeshProcessor::completePolygonPolyhedronRelations(test_model);

	rmodel->loadRModelData(test_model);
	sel.setRModel(rmodel);

	// External faces Ids
	int surfacePolygonIndeces[12] = {1, 4, 3, 6, 8, 9, 11, 12, 13, 15, 16, 17};
	sel.setSelectionType(vis::CONSTANTS::POLYGON);
	std::vector<vis::Polygon>& polygons = test_model->getPolygons();
	for (int id : surfacePolygonIndeces){
		sel.addSelectedElement(&polygons[id]);
		}
	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	QRadioButton* selectionMode = config->findChild<QRadioButton*>("radioButton_all");
	selectionMode->setChecked(true);

	tested->setup();
	tested->selectElement(&sel);
	QCOMPARE(sel.getSelectedElements().size(), 0u);
	}

/***
*  @brief Test polygon transition between elements in the case of inclusive selection
*
*  Same setup as previous test, in this case the selection should pick all tetrahedrons
*  in the model. As all tetrahedrons have a face in the surface of the cube.
*/
void ChangeSelectionTypeTest::polygonToPolyhedronInclusive(){
	UnitCube cube(true);
	Selection sel = Selection();
	Model* test_model = cube.polyhedronmesh;
	MeshProcessor::completePolygonPolyhedronRelations(test_model);

	rmodel->loadRModelData(test_model);
	sel.setRModel(rmodel);

	// External faces Ids
	int surfacePolygonIndeces[12] = {1, 3, 4, 6, 8, 9, 11, 12, 13, 15, 16, 17};
	std::vector<vis::Polygon>& polygons = test_model->getPolygons();
	sel.setSelectionType(vis::CONSTANTS::POLYGON);
	for (int id : surfacePolygonIndeces){
		sel.addSelectedElement(&polygons[id]);
		}

	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polyhedron");
	modelType->setChecked(true);

	QRadioButton* selectionMode = config->findChild<QRadioButton*>("radioButton_at_least1");
	selectionMode->setChecked(true);

	tested->setup();
	tested->selectElement(&sel);
	QCOMPARE(sel.getSelectedElements().size(), 6u);
	}

/***
*  @brief Test polyhedron transition between elements in the case of exclusive selection
*
*  Test case starts by selecting a single tetrahedron from the test model, in theory
*  the selection should keep all the faces of the tetrahedron, but in practice
*  the program will discard the two internal faces of the element, as both have
*  a complementary polyhedron that is not selected.
*
*  @note Identify if this is the intended behavior and test accordingly
*/
void ChangeSelectionTypeTest::polyhedronToPolygonExclusive(){
	UnitCube cube(true);
	Selection sel = Selection();
	Model* test_model = cube.polyhedronmesh;
	MeshProcessor::completePolygonPolyhedronRelations(test_model);

	rmodel->loadRModelData(test_model);
	sel.setRModel(rmodel);

	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	sel.addSelectedElement((vis::Element*)&test_model->getPolyhedrons()[0]);

	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polygon");
	modelType->setChecked(true);

	QRadioButton* selectionMode = config->findChild<QRadioButton*>("radioButton_all");
	selectionMode->setChecked(true);

	tested->setup();
	tested->selectElement(&sel);

	QCOMPARE(sel.getSelectedElements().size(), 2u);
	}


/***
*  @brief Test polyhedron transition between elements in the case of exclusive selection
*
*  Test case starts by selecting a single tetrahedron from the test model, in this
*  case setting the selection will remove the internal face in the mesh.
*
*  @note Identify if this is the intended behavior and test accordingly
*/
void ChangeSelectionTypeTest::polyhedronToPolygonInclusive(){
	UnitCube cube(true);
	Selection sel = Selection();
	Model* test_model = cube.polyhedronmesh;
	MeshProcessor::completePolygonPolyhedronRelations(test_model);

	rmodel->loadRModelData(test_model);
	sel.setRModel(rmodel);

	sel.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	sel.addSelectedElement((vis::Element*)&test_model->getPolyhedrons()[0]);

	QRadioButton* modelType = config->findChild<QRadioButton*>("radio_polygon");
	modelType->setChecked(true);

	QRadioButton* selectionMode = config->findChild<QRadioButton*>("radioButton_at_least1");
	selectionMode->setChecked(true);

	tested->setup();
	tested->selectElement(&sel);
	QCOMPARE(sel.getSelectedElements().size(), 4u);
	}

void ChangeSelectionTypeTest::cleanupTestCase(){
	delete tested;
	delete context;
	delete rmodel;
	}