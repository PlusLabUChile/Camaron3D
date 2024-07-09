#include "test_selectbyid.h"
#include "Test/Utils/unitcube.h"
#include "Common/Constants.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include "SelectionStrategies\Selection.h"
#include "SelectionStrategies\SelectById\selectbyid.h"
#include "SelectionStrategies\SelectById\selectbyidconfig.h"

void SelectByIdTest::initTestCase(){
	tested = new SelectById();
	tested->QApplicationInitiated();
	config = (SelectByIdConfig*)tested->getConfigWidget();
	rmodel = new RModel();
	}

void SelectByIdTest::selectElements(){
	Selection contents = Selection();
	
	UnitCube cube(false);
	Model* test_model = cube.vertexcloud;

	rmodel->loadRModelData(test_model);
	contents.setRModel(rmodel);

	// Set range to be requested (First half inclusive)
	config->minId = 0;
	int half = test_model->getVerticesCount()/2;
	config->maxId = half;

	// Attempt to select everything with the strategy
	for (vis::Vertex& vert : test_model->getVertices()){
		tested->selectElement(&vert, &contents);
		}

	// Verify that only half was actually selected
	std::unordered_map<int, vis::Element*>& selected = contents.getSelectedElements();
	QCOMPARE(selected.size(), (unsigned int)half + 1);

	for (int i = 0; i <= half; i++){
		QCOMPARE(i, selected[i]->getId());
		}
	}

void SelectByIdTest::setupNewSelection(){
	Selection container = Selection();
	container.setRModel(rmodel);

	config->selectionType = SelectByIdConfig::NEW_SELECTION;
	config->elementsType = vis::CONSTANTS::POLYGON;

	vis::Vertex vert = vis::Vertex(0, 0.0f, 0.0f, 0.0f);
	container.addSelectedElement((vis::Element*)&vert);

	// A new selection should remove all previous elements and change the element type
	// in the selection object
	tested->setupPreApplying(&container, rmodel);

	QVERIFY(container.getSelectedElements().size() == 0);
	QVERIFY(container.getSelectionType() == vis::CONSTANTS::POLYGON);
	}

void SelectByIdTest::setupOtherSelection(){
	Selection container = Selection();
	config->selectionType = SelectByIdConfig::JOIN_SELECTION;

	// Attempting to use an uninitialized selection should return false
	QVERIFY(!tested->setupPreApplying(&container, (RModel*)0));

	container.setSelectionType(vis::CONSTANTS::POLYGON);
	QVERIFY(tested->setupPreApplying(&container, (RModel*)0));
	}

void SelectByIdTest::cleanupTestCase(){
	delete tested;
	delete rmodel;
	}