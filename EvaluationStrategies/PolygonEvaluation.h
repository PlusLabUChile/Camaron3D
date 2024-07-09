#ifndef POLYGONEVALUATION_H
#define POLYGONEVALUATION_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class PolygonEvaluation: public EvaluationStrategy
{
	public:
		PolygonEvaluation(std::string, std::string);
		PolygonEvaluation(std::string, std::string, std::string);
		virtual ~PolygonEvaluation();
		virtual float getNullValue() = 0;
		virtual void setup(Model* model);
		virtual int getSupportedType();
		virtual void evaluateElementsFrom(Model* model);
		virtual void requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target);
};

#endif // POLYGONEVALUATION_H
