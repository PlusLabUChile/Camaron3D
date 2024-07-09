#ifndef MAXIMUMINTERNALANGLE_H
#define MAXIMUMINTERNALANGLE_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class MaximumInternalAngle: public PolygonEvaluation
{
	public:
		MaximumInternalAngle();
		virtual ~MaximumInternalAngle();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};
#endif // MAXIMUMINTERNALANGLE_H
