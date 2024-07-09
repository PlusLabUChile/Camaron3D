#include "VertexEvaluation.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"

VertexEvaluation::VertexEvaluation(std::string name, std::string propertyName):
	EvaluationStrategy(name, propertyName){}

VertexEvaluation::VertexEvaluation(std::string name, std::string description, std::string propertyName):
	EvaluationStrategy(name, description, propertyName){}

VertexEvaluation::~VertexEvaluation(){}

void VertexEvaluation::setup(Model* model){
	setTotalElementsCount(model->getVerticesCount());
	}

int VertexEvaluation::getSupportedType(){
	return vis::CONSTANTS::VERTEX;
	}

void VertexEvaluation::evaluateElementsFrom(Model* model){
	applyEvaluation(model, model->getVertices());
	this->applied = true;
	}

void VertexEvaluation::requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target){
	if(!this->applied){
		evaluateElementsFrom(model);
		}
	mapResultsToRModel(model->getVertices(), target);
	}
