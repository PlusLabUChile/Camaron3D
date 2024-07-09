#include "SelectionStrategies/SelectionStrategyByProperty/selectionstrategybyproperty.h"
#include "SelectionStrategies/SelectionStrategyByProperty/selectionstrategybypropertyconfig.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Vertex.h"
#include "Common/Constants.h"
SelectionStrategyByProperty::SelectionStrategyByProperty():
	SelectionStrategy("Select by Property")
{
	config = (SelectionStrategyByPropertyConfig*)0;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
}

SelectionStrategyByProperty::~SelectionStrategyByProperty()
{
	if(config)
		delete config;
}

bool SelectionStrategyByProperty::selectElement(vis::Polyhedron * p, Selection* sel) {
	Model* model = sel->getRModel()->getOriginalModel();
    std::vector<float>& evaluationData = evaluationStrategy->getEvaluationResults(model);
    float value = evaluationData[p->getId()];
    if( value != evaluationStrategy->getNullValue()
		&& this->minVal <= value
		&& this->maxVal >= value){
        sel->addSelectedElement(p);
		return true;
		}
	return false;
	}

bool SelectionStrategyByProperty::selectElement(vis::Polygon * p, Selection* sel) {
    Model* model = sel->getRModel()->getOriginalModel();
	std::vector<float>& evaluationData = evaluationStrategy->getEvaluationResults(model);
    float value = evaluationData[p->getId()];
    if( value != evaluationStrategy->getNullValue()
		&& this->minVal <= value
		&& this->maxVal >= value){
        sel->addSelectedElement(p);
		return true;
		}
	return false;
	}

bool SelectionStrategyByProperty::selectElement(vis::Vertex * p, Selection* sel) {
	Model* model = sel->getRModel()->getOriginalModel();
    std::vector<float>& evaluationData = evaluationStrategy->getEvaluationResults(model);
    float value = evaluationData[p->getId()];
    if( value != evaluationStrategy->getNullValue()
		&& this->minVal <= value
		&& this->maxVal >= value){
        sel->addSelectedElement(p);
		return true;
		}
	return false;
	}

bool SelectionStrategyByProperty::isFullFilled(vis::Vertex*){
	return evaluationStrategy->getSupportedType() == vis::CONSTANTS::VERTEX;
	}
bool SelectionStrategyByProperty::isFullFilled(vis::Polygon*) {
	return evaluationStrategy->getSupportedType() == vis::CONSTANTS::POLYGON;
	}
bool SelectionStrategyByProperty::isFullFilled(vis::Polyhedron*) {
	return evaluationStrategy->getSupportedType() == vis::CONSTANTS::POLYHEDRON;
	}

bool SelectionStrategyByProperty::setup(){
	config->readConfig();
	this->minVal = config->minVal;
	this->maxVal = config->maxVal;
	this->evaluationStrategy = config->evaluationStrategy;
	this->applyOnSelection = !config->applyOverModel;
	return true;
	}

bool SelectionStrategyByProperty::setupPreApplying(Selection* sel, RModel *){
	if(!this->applyOnSelection){
		if(config->newSelection){
			sel->clearSelectedElements();
			sel->setSelectionType(this->evaluationStrategy->getSupportedType());
			}
		else{
			if(sel->getSelectedElements().empty()){
				sel->setSelectionType(this->evaluationStrategy->getSupportedType());
				}
			else{
				return sel->getSelectionType() == this->evaluationStrategy->getSupportedType();
				}
			}
		}
	return true;
	}

bool SelectionStrategyByProperty::hasConfigWidget(){
	return true;
}

QWidget* SelectionStrategyByProperty::getConfigWidget(){
	return this->config;
}
void SelectionStrategyByProperty::QApplicationInitiated(){
	config = new SelectionStrategyByPropertyConfig();
}

#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(SelectionStrategyByProperty);
