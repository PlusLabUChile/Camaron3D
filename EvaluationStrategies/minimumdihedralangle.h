#ifndef MINIMUMDIHEDRALANGLE_H
#define MINIMUMDIHEDRALANGLE_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class MinimumDihedralAngle: public PolygonEvaluation
{
	public:
		MinimumDihedralAngle();
		virtual ~MinimumDihedralAngle();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		virtual void evaluateElementsFrom(Model* model);
		void QApplicationInitiatedEv();
};

#endif // MINIMUMDIHEDRALANGLE_H
