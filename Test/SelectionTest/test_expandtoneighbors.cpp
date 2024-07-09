#include "test_expandtoneighbors.h"
#include "Test/Utils/unitcube.h"
#include "Common/Constants.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelLoading/MeshProcessor.h"
#include "SelectionStrategies\Selection.h"
#include "SelectionStrategies\ExpandToNeighbors\expandtoneighbors.h"
#include "SelectionStrategies\ExpandToNeighbors\expandtoneighborsconfig.h"
#include <QRadioButton>
#include <QLineEdit>

void ExpandToNeighborsTest::initTestCase(){
	tested = new ExpandToNeighbors();
	tested->QApplicationInitiated();
	config = (ExpandToNeighborsConfig*)tested->getConfigWidget();
	config->onlySurface = false;
	config->onlySurfaceWithAngle = false;
	config->dihedralAngle = 0.6f;
	
	rmodel = new RModel();
	}

void ExpandToNeighborsTest::init(){
	/// Setup intial model
	cube = new UnitCube(true);
	test_model = cube->polyhedronmesh;

	// Compute neighbor elements
	MeshProcessor::completeVertexPolygonRelations(test_model);
	MeshProcessor::completePolygonPolygonRelations(test_model);
	MeshProcessor::completePolygonPolyhedronRelations(test_model);

	// To obtain the dihedral angles, we require that face normals are computed
	MeshProcessor::calculateNormalsPolygons(test_model);
	}

/***
*  @brief Polygon expansion without limitation
*
*  Test case picks a face at the surface of the unit cube, after expansion
*  this should add internal faces to the selection as well as any surrounding face
*/
void ExpandToNeighborsTest::expandAllPolygons(){
	Selection selection = Selection();
	rmodel->loadRModelData(test_model);

	selection.setRModel(rmodel);
	selection.setSelectionType(vis::CONSTANTS::POLYGON);
	selection.addSelectedElement(&test_model->getPolygons()[1]);

	config->onlySurface = false;
	config->onlySurfaceWithAngle = false;

	tested->selectElement(&selection);

	int expectedFaceIds[6] = {0, 1, 2, 3, 4, 8};
	std::unordered_map<int, vis::Element*>& result = selection.getSelectedElements();
	QCOMPARE(result.size(), 6u);

	for (int id : expectedFaceIds){
		QVERIFY(result[id] != nullptr);
		QCOMPARE(result[id]->getId(), id);
		}
	}

void ExpandToNeighborsTest::expandAllPolyhedrons(){
	QSKIP("Require test model with full internal polyhedrons");
	}

/***
*  @brief Polygon expansion with surface limitation
*
*  Expanding the first surface triangle should not select any internal face when
*  using this settings
*/
void ExpandToNeighborsTest::expandSurfacePolygons(){
	Selection selection = Selection();
	rmodel->loadRModelData(test_model);

	selection.setRModel(rmodel);
	selection.setSelectionType(vis::CONSTANTS::POLYGON);
	selection.addSelectedElement(&test_model->getPolygons()[1]);

	config->onlySurface = true;
	config->onlySurfaceWithAngle = false;

	tested->selectElement(&selection);

	int expectedFaceIds[4] = {1, 3, 4, 8};
	std::unordered_map<int, vis::Element*>& result = selection.getSelectedElements();
	QCOMPARE(result.size(), 4u);

	for (int id : expectedFaceIds){
		QVERIFY(result[id] != nullptr);
		QCOMPARE(result[id]->getId(), id);
		}
	}

//Same as above
void ExpandToNeighborsTest::expandSurfacePolyhedrons(){
	Selection selection = Selection();
	rmodel->loadRModelData(test_model);

	selection.setRModel(rmodel);
	selection.setSelectionType(vis::CONSTANTS::POLYHEDRON);
	selection.addSelectedElement((vis::Element*)&test_model->getPolyhedrons()[1]);

	config->onlySurface = true;
	config->onlySurfaceWithAngle = false;

	tested->selectElement(&selection);

	int expectedFaceIds[3] = {0, 1, 4};
	std::unordered_map<int, vis::Element*>& result = selection.getSelectedElements();
	QCOMPARE(result.size(), 3u);

	for (int id : expectedFaceIds){
		QVERIFY(result[id] != nullptr);
		QCOMPARE(result[id]->getId(), id);
		}
	}

/***
*  @brief Polygon expansion with dihedral angle limitations
*
*  Expanding the first surface triangle with the default angle (0.5f) will only
*  allow adding the other triangle in the cube face. No other triangles can be
*  accessed this way
*/
void ExpandToNeighborsTest::expandSurfaceWithAnglePolygons(){
	Selection selection = Selection();
	rmodel->loadRModelData(test_model);

	selection.setRModel(rmodel);
	selection.setSelectionType(vis::CONSTANTS::POLYGON);
	selection.addSelectedElement(&test_model->getPolygons()[1]);

	config->onlySurface = false;
	config->onlySurfaceWithAngle = true;

	tested->selectElement(&selection);
	QCOMPARE(selection.getSelectedElements().size(), 2u);
	QVERIFY(selection.getSelectedElements()[4] != nullptr);

	// Attempt to expand again should do nothing
	tested->selectElement(&selection);
	QCOMPARE(selection.getSelectedElements().size(), 2u);
	}


/***
*  @brief Polyhedron expansion with dihedral angle limitations
*
*/
void ExpandToNeighborsTest::expandSurfaceWithAnglePolyhedrons(){
	QSKIP("Bugged implementation, unsure about expected behavior");
	}

void ExpandToNeighborsTest::cleanup(){
	delete cube;
	}

void ExpandToNeighborsTest::cleanupTestCase(){
	delete tested;
	delete rmodel;
	}