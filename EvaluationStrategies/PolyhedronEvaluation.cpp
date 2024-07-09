#include "PolyhedronEvaluation.h"
#include "Model/Model.h"
#include "Model/Element/Polyhedron.h"

PolyhedronEvaluation::PolyhedronEvaluation(std::string name, std::string propertyName):
	EvaluationStrategy(name, propertyName){}

PolyhedronEvaluation::PolyhedronEvaluation(std::string name, std::string description, std::string propertyName):
	EvaluationStrategy(name, description, propertyName){}

PolyhedronEvaluation::~PolyhedronEvaluation(){}

int PolyhedronEvaluation::getSupportedType(){
	return vis::CONSTANTS::POLYHEDRON;
	}

void PolyhedronEvaluation::setup(Model* model){
	setTotalElementsCount(model->getPolyhedronsCount());
	}

void PolyhedronEvaluation::evaluateElementsFrom(Model* model){
	applyEvaluation(model, model->getPolyhedrons());
	this->applied = true;
	}

void PolyhedronEvaluation::requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target){
	if(!this->applied){
		evaluateElementsFrom(model);
		}
	mapResultsToRModel(model->getPolyhedrons(), target);
	}
