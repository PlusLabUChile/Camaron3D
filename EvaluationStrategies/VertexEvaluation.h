#ifndef VERTEXEVALUATION_H
#define VERTEXEVALUATION_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class VertexEvaluation: public EvaluationStrategy
{
	public:
		VertexEvaluation(std::string, std::string);
		VertexEvaluation(std::string, std::string, std::string);
		virtual ~VertexEvaluation();
		virtual float getNullValue() = 0;
		virtual void setup(Model* model);
		virtual int getSupportedType();
		virtual void evaluateElementsFrom(Model* model);
		virtual void requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target);
};

#endif // VERTEXEVALUATION_H
