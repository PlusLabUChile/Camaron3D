#ifndef ELEMENT_H
#define ELEMENT_H
#include <vector>
#include <glm/glm.hpp>

#define DOUBLE_DISPATCH_ELEMENT_DEC0 \
	virtual bool applySelectionStrategyDD(SelectionStrategy*, Selection*) = 0;
#define DOUBLE_DISPATCH_ELEMENT_DEC \
	virtual bool applySelectionStrategyDD(SelectionStrategy*, Selection*);
#define DOUBLE_DISPATCH_ELEMENT_DEF(x) \
	bool x::applySelectionStrategyDD(SelectionStrategy* stra, Selection* sel){\
	return (stra->isFullFilled(this))?stra->selectElement(this,sel):false;\
	}

class SelectionStrategy;
class EvaluationStrategy;
class Selection;
namespace vis{

class Element{
	public:
		Element(unsigned int);
		virtual ~Element();
		unsigned int getId();
		bool isSelected();
		void setSelected(bool);
		virtual glm::vec3 getGeometricCenter() = 0;

		//rmodel
		void addRmodelPositions(int pos);
		std::vector<int>& getRmodelPositions();

		//Double Dispatch
		DOUBLE_DISPATCH_ELEMENT_DEC0
	protected:
		bool selected;
		unsigned int id;
	private:
		std::vector<int> rmodelPositions;
};
}
#endif // ELEMENT_H
