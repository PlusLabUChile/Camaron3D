#ifndef MINIMUMINTERNALANGLE_H
#define MINIMUMINTERNALANGLE_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class MinimumInternalAngle: public PolygonEvaluation
{
	public:
		MinimumInternalAngle();
		virtual ~MinimumInternalAngle();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // MINIMUMINTERNALANGLE_H
