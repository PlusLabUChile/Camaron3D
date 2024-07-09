#ifndef MAXIMUMSOLIDANGLE_H
#define MAXIMUMSOLIDANGLE_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolyhedronEvaluation.h"
class MaximumSolidAngle: public PolyhedronEvaluation
{
	public:
		MaximumSolidAngle();
		virtual ~MaximumSolidAngle();
		virtual void value(Model* model, vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MAXIMUMSOLIDANGLE_H
