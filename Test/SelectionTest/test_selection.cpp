#include "test_selection.h"
#include "Rendering/RModel/rmodel.h"
#include "Test/Utils/openglcontext.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "Test/Utils/unitcube.h"
#include "SelectionStrategies/Selection.h"
#include "ModelLoading/MeshProcessor.h"

void SelectionTest::initTestCase(){

	/// @note Since we are using a RModel instance, we must start an OpenGL
	/// context at the beginning, allowing us to use its internal opengl calls.

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
	}

void SelectionTest::init(){
	tested = new Selection();
	model = new UnitCube(true);
	test_model = model->polyhedronmesh;

	rmodel->loadRModelData(test_model);
	tested->setRModel(rmodel);
	}


/***
*  @brief Following 3 tests should insert each element type into a Selection
*
*  Setup requires to load the test model into the common RModel instance, then
*  we proceed to pass a pointer to the rmodel to the tested selection, while
*  setting the appropriate element type inside the container. After this
*  we call verifySelectionInsert to continue the test
*
*  @todo Test with some elements of the vectors and verify that the other elements
*         are not affected by the insertions
*/
void SelectionTest::addSelectedVertices(){
	tested->setSelectionType(0);
	std::vector<vis::Vertex>& vertices = test_model->getVertices();

	verifySelectionInsert(vertices);
	}

void SelectionTest::addSelectedPolygons(){
    tested->setSelectionType(1);
    std::vector<vis::Polygon>& polygons = test_model->getPolygons();

    verifySelectionInsert(polygons);
	}

void SelectionTest::addSelectedPolyhedrons(){
    tested->setSelectionType(2);
    std::vector<vis::Polyhedron>& polyhedrons = test_model->getPolyhedrons();

    verifySelectionInsert(polyhedrons);
	}

/***
*  @brief Test capability of a Selection to store more than one type
*
*  This test handles the ideal case where both elements don't have the same internal
*  Id, we already tested above the side effects of insertions, so we should only
*  need to verify that its contents are correct.
*
*  @note Deprecate this test after implementing exclusive selection
*/
void SelectionTest::addSelectedMix(){
	tested->setSelectionType(1);
	vis::Vertex* vertex = &test_model->getVertices()[0];
	vis::Polygon* polygon = &test_model->getPolygons()[1];

	tested->addSelectedElement(vertex);
	tested->addSelectedElement(polygon);

	std::unordered_map<int, vis::Element*>& content = tested->getSelectedElements();

	QVERIFY(content.size() == 2);
	QVERIFY(content[0] != nullptr && content[0]->getId() == vertex->getId());
	QVERIFY(content[1] != nullptr && content[1]->getId() == polygon->getId());
	}

/***
*  @brief Test container behavior with elements of different type but with the
*  same ID
*
*  In theory, selections strategies that are type agnostic (ex: extendtoneighbors)
*  should be able to operate with mixed selections, but considering that element
*  collections are stored with Ids starting from the same number, there is a chance
*  of ID collision issues between entries, as the internal hash map in the Selection
*  enforces uniqueness between keys. The current implementation will override the previous
*  pointer with the newly inserted one, causing selected elements to be discarded.
*
*  @note Deprecate this test after implementing exclusive selection
*
*/
void SelectionTest::addSelectedWithIdCollisions(){
	tested->setSelectionType(1);
	vis::Vertex* vertex = &test_model->getVertices()[0];
	vis::Polygon* polygon = &test_model->getPolygons()[0];

	tested->addSelectedElement(vertex);
	tested->addSelectedElement(polygon);

	std::unordered_map<int, vis::Element*>& content = tested->getSelectedElements();
	QEXPECT_FAIL("", "Collision between elements", Abort);
	QVERIFY(content.size() == 2);
	///@note How should we access both instances if the key is the same?
	QVERIFY(content[0] != nullptr && content[0]->getId() == vertex->getId());
	QVERIFY(content[1] != nullptr && content[1]->getId() == polygon->getId());
	}


/***
*  @brief Test for Selection::clearSelectedElements method
*
*  Replicates use case where we need to eliminate all elements in the selection
*  while the RModel and Model instances are still active (ex: select empty area).
*/
void SelectionTest::clearSelectedWithSideEffects(){
	for (vis::Polygon& poly : test_model->getPolygons()){
		tested->addSelectedElement(&poly);
		}

	tested->clearSelectedElements();
	QVERIFY(tested->getSelectedElements().size() == 0);

	for (vis::Polygon& poly : test_model->getPolygons()){
		QVERIFY(!poly.isSelected());
		}

	for (RVertexFlagAttribute rvert : rmodel->vertexFlagsAttribute){
		QVERIFY(!rvert.isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
		}
	}

/***
*  @brief Test for Selection::reset method
*
*  This method is used in the case where the Model and the RModel data where
*  deleted before the Selection is cleared (ex: Model change), clearing the
*  selection should not access the previous instances to prevent segfaults.
*/
void SelectionTest::clearSelectedWithoutSideEffects(){
	for (vis::Polygon& poly : test_model->getPolygons()){
		tested->addSelectedElement(&poly);
		}

	tested->reset();
	QVERIFY(tested->getSelectedElements().size() == 0);

	// To prevent crashing the tests, we will verify that the RModel and Model instances
	// are still the same after the reset

	for (vis::Polygon& poly : test_model->getPolygons()){
		QVERIFY(poly.isSelected());
		}

	for (RVertexFlagAttribute rvert : rmodel->vertexFlagsAttribute){
		QVERIFY(rvert.isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
		}
	}

/***
*  @brief Removal of isolated entries
*
*  In this use case, we select two elements that have no common elements and
*  we will delete one. In this case the method should revert the side effects
*  from insertion for any element type.
*
*  @todo Test for each element type
*/
void SelectionTest::deleteSelectedIsolatedElement(){
	// Pick isolated triangles
	vis::Polygon* polyA = &test_model->getPolygons()[4];
	vis::Polygon* polyB = &test_model->getPolygons()[9];
	tested->addSelectedElement(polyA);
	tested->addSelectedElement(polyB);

	tested->removeSelectedElement(polyB);

	std::vector<int>& polyAIndeces = polyA->getRmodelPositions();
	std::vector<int>& polyBIndeces = polyB->getRmodelPositions();

	std::vector<RVertexFlagAttribute>& flags = rmodel->vertexFlagsAttribute;
	for (int index : polyAIndeces){
		QVERIFY(flags[index].isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
		}

	for (int index : polyBIndeces){
		QVERIFY(!flags[index].isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
		}
	}

/***
*  @brief Removal of polyhedrons with shared faces
*
*  Elements with shared vertices/faces will have common RVertexFlags in the
*  RModel representation. For polyhedrons deleting an instance should prevent
*  the deletion of common faces to prevent neighbor polyhedrons to lack
*  shared faces.
*/
void SelectionTest::deleteSelectedSharedFace(){

	// Need to compute polyhedron neighbors beforehand
	MeshProcessor proc = MeshProcessor();
	proc.completePolygonPolyhedronRelations(test_model);

	// Pick polyhedrons with a shared face (Polygon 2 in this case)
	vis::Polyhedron* polyhA = &test_model->getPolyhedrons()[0];
	vis::Polyhedron* polyhB = &test_model->getPolyhedrons()[1];
	tested->addSelectedElement(polyhA);
	tested->addSelectedElement(polyhB);

	tested->removeSelectedElement(polyhB);

	std::vector<RVertexFlagAttribute>& flags = rmodel->vertexFlagsAttribute;

	//Polyhedron A: indeces from 0 to 11
	//Polyhedron B: indeces from 6 to 20

	for (int index = 0; index < 20; index++){
		if (index <= 11){
			QVERIFY(flags[index].isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
			}
		else {
			QVERIFY(!flags[index].isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
			}
		}
	}

/***
*  @brief Removal of polygons with shared vertices
*
*  Elements with shared vertices/faces will have common RVertexFlags in the
*  RModel representation. For polygons deleting the Selection object will NOT
*  attempt to prevent shared vertex deletion. This is the expected behavior
*  of the current implementation, as vertex are not the major focus of selections
*  at the time.
*
*/
void SelectionTest::deleteSelectedSharedVertex(){
	QSKIP("Low Priority edge case");
	}

void SelectionTest::cleanup(){
	delete tested;
	delete model;
	}

void SelectionTest::cleanupTestCase(){
	delete context;
	delete rmodel;
	}