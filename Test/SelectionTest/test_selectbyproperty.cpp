#include "test_selectbyproperty.h"
#include "Test/Utils/unitcube.h"
#include "Common/Constants.h"
#include "Model/Element/Element.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include "SelectionStrategies\Selection.h"
#include "SelectionStrategies\SelectionStrategyByProperty\selectionstrategybyproperty.h"
#include "SelectionStrategies\SelectionStrategyByProperty\selectionstrategybypropertyconfig.h"
#include <QLineEdit>
#include <QRadioButton>
#include <QComboBox>

void SelectByPropertyTest::initTestCase(){
	tested = new SelectionStrategyByProperty();
	tested->QApplicationInitiated();
	config = (SelectionStrategyByPropertyConfig*)tested->getConfigWidget();
	
	rmodel = new RModel();
	}

/***
*  @note Similar to SelectById, internal implementation is the same, add more
*  cases per type later to ensure interface consistency in case of future changes
*/

void SelectByPropertyTest::selectElement(){
	Selection contents = Selection();
	UnitCube cube(true);
	Model* test_model = cube.polygonmesh;
	rmodel->loadRModelData(test_model);
	contents.setRModel(rmodel);
	/// For the tetra cube, surface area has 6 polygons with slightly bigger areas
    /// than the rest.
	/// @todo Polygon Area is currently the default, set it anyway in case of change

	QLineEdit* minimum = config->findChild<QLineEdit*>("ssbp_minimumValueInputSelect");
	minimum->setText("0.6");

	QLineEdit* maximum = config->findChild<QLineEdit*>("ssbp_maximumValueInputSelect");
	maximum->setText("0.8");

	tested->setup();

	// Attempt to select everything with the strategy
	for (vis::Polygon& poly : test_model->getPolygons()){
		tested->selectElement(&poly, &contents);
		}

	// Verify that the expected polygons where selected
	int expectedIds[6] = {0, 2, 5, 7, 10, 14};
	std::unordered_map<int, vis::Element*>& selected = contents.getSelectedElements();
	QCOMPARE(selected.size(), 6u);

	for (int id : expectedIds){
		QVERIFY(selected[id] != nullptr);
		}
	}

void SelectByPropertyTest::setupNewSelection(){
	Selection container = Selection();
	container.setRModel(rmodel);
	QRadioButton* newSelection = config->findChild<QRadioButton*>("radioNewSelection");
	newSelection->setChecked(true);

	vis::Vertex vert = vis::Vertex(0, 0.0f, 0.0f, 0.0f);
	container.setSelectionType(vis::CONSTANTS::VERTEX);
	container.addSelectedElement((vis::Element*)&vert);

	// A new selection should remove all previous elements and change the element type
	// in the selection object
	tested->setup();
	tested->setupPreApplying(&container, rmodel);

	QVERIFY(container.getSelectedElements().size() == 0);
	QVERIFY(container.getSelectionType() == vis::CONSTANTS::POLYGON);
	}

void SelectByPropertyTest::setupUnionSelection(){
	Selection container = Selection();
	container.setRModel(rmodel);

	QComboBox* evaluations = config->findChild<QComboBox*>("comboBoxEvalStrategiesSelect");
	// Pick an evaluation strategy which uses polygons
	evaluations->setCurrentIndex(2);

	///@note To disable the new selection option, we must click the union button instead
	QRadioButton* unionSelection = config->findChild<QRadioButton*>("radioUnionSelection");
	unionSelection->setChecked(true);

	vis::Polygon poly = vis::Polygon(0);
	container.setSelectionType(vis::CONSTANTS::POLYGON);
	container.addSelectedElement((vis::Element*)&poly);

	tested->setup();

	// Selection should keep the previous selection intact
	QVERIFY(tested->setupPreApplying(&container, rmodel));
	QVERIFY(container.getSelectionType() == vis::CONSTANTS::POLYGON);
	QVERIFY(container.getSelectedElements().size() == 1u);
	}

void SelectByPropertyTest::cleanupTestCase(){
	delete tested;
	delete rmodel;
	}