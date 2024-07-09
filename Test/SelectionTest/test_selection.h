#ifndef TEST_SELECTION_H
#define TEST_SELECTION_H

#include <QTest>
#include "SelectionStrategies\Selection.h"
#include "Model/Element/Element.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include "Rendering/RModel/rmodel.h"

class OpenGLContext;
class UnitCube;
class Model;

class SelectionTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void init();
		void addSelectedVertices();
		void addSelectedPolygons();
		void addSelectedPolyhedrons();
		void addSelectedMix();
		void addSelectedWithIdCollisions();

		void clearSelectedWithSideEffects();
		void clearSelectedWithoutSideEffects();
		void deleteSelectedIsolatedElement();
		void deleteSelectedSharedFace();
		void deleteSelectedSharedVertex();
		void cleanup();
		void cleanupTestCase();

	private:
		template <typename ELEMENT_TYPE>
		void verifySelectionInsert(std::vector<ELEMENT_TYPE>& elements);

		RModel* rmodel;
		OpenGLContext* context;
		UnitCube* model;
		Selection* tested;
		Model* test_model;
	};

/***
* @brief Verify that each element insertion side-effect is correctly applied
*
* Given a vector of elements to be inserted and a pointer to the Selection object
* which will contain them, this method will insert each element sequentially,
* after each insertion the following side effects must be corroborated:
*
* I   - The inserted element internal flag "isSelected" is set to true
* II  - A map entry inside the Selection pairs the id of the element with a pointer to
*       said element.
* III - The element positions inside the RVertexFlag arrays should be set to SELECTED_FLAG
*
*/
template <typename ELEMENT_TYPE>
void SelectionTest::verifySelectionInsert(std::vector<ELEMENT_TYPE>& elements){
	std::unordered_map<int, vis::Element*>& selected = tested->getSelectedElements();
	for (ELEMENT_TYPE& ele : elements){
		tested->addSelectedElement(&ele);

		//I
		QVERIFY(ele.isSelected());

		//II
		vis::Element* contained_element = selected[ele.getId()];
		QVERIFY(contained_element != nullptr);
		QVERIFY(contained_element->getId() == ele.getId());

		//III
		std::vector<int>& rvertexIndeces = ele.getRmodelPositions();
		for (int index : rvertexIndeces){
			RVertexFlagAttribute rvert = rmodel->vertexFlagsAttribute[index];
			QVERIFY(rvert.isFlagEnabled(RVertexFlagAttribute::SELECTED_FLAG));
			}
		}
	}

#endif