#ifndef POLYHEDRONEVALUATION_H
#define POLYHEDRONEVALUATION_H
#include "EvaluationStrategies/EvaluationStrategy.h"
class PolyhedronEvaluation: public EvaluationStrategy
{
	public:
		PolyhedronEvaluation(std::string, std::string);
		PolyhedronEvaluation(std::string, std::string, std::string);
		virtual ~PolyhedronEvaluation();
		virtual float getNullValue() = 0;
		virtual void setup(Model* model);
		virtual int getSupportedType();
		virtual void evaluateElementsFrom(Model* model);
		virtual void requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target);
};

#endif // POLYHEDRONEVALUATION_H
