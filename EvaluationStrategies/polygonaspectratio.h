#ifndef POLYGONASPECTRATIO_H
#define POLYGONASPECTRATIO_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"

class PolygonAspectRatio: public PolygonEvaluation
{
	public:
		PolygonAspectRatio();
		virtual ~PolygonAspectRatio();
		virtual void value(Model* model, vis::Polygon* m);
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYGONASPECTRATIO_H
