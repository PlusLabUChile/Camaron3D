#include "PolygonEvaluation.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"

PolygonEvaluation::PolygonEvaluation(std::string name, std::string propertyName):
	EvaluationStrategy(name, propertyName){}

PolygonEvaluation::PolygonEvaluation(std::string name, std::string description, std::string propertyName):
	EvaluationStrategy(name, description, propertyName){}

PolygonEvaluation::~PolygonEvaluation(){}

int PolygonEvaluation::getSupportedType(){
	return vis::CONSTANTS::POLYGON;
	}

void PolygonEvaluation::setup(Model* model){
	setTotalElementsCount(model->getPolygonsCount());
	}

void PolygonEvaluation::evaluateElementsFrom(Model* model){
	applyEvaluation(model, model->getPolygons());
	this->applied = true;
	}

void PolygonEvaluation::requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target){
	if(!this->applied){
		evaluateElementsFrom(model);
		}
	mapResultsToRModel(model->getPolygons(), target);
	}
