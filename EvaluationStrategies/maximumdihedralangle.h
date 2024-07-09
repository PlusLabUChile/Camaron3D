#ifndef MAXIMUMDIHEDRALANGLE_H
#define MAXIMUMDIHEDRALANGLE_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class MaximumDihedralAngle: public PolygonEvaluation
{
	public:
		MaximumDihedralAngle();
		virtual ~MaximumDihedralAngle();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		virtual void evaluateElementsFrom(Model* model);
		void QApplicationInitiatedEv();
};

#endif // MAXIMUMDIHEDRALANGLE_H
