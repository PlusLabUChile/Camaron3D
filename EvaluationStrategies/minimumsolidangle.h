#ifndef MINIMUMSOLIDANGLE_H
#define MINIMUMSOLIDANGLE_H

#include "EvaluationStrategies/PolyhedronEvaluation.h"
class MinimumSolidAngle: public PolyhedronEvaluation
{
	public:
		MinimumSolidAngle();
		virtual ~MinimumSolidAngle();
		virtual void value(Model* model, vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MINIMUMSOLIDANGLE_H
