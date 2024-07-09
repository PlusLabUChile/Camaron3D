#ifndef POLYGONVERTICESCOUNT_H
#define POLYGONVERTICESCOUNT_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"

class PolygonVerticesCount: public PolygonEvaluation
{
	public:
		PolygonVerticesCount();
		virtual ~PolygonVerticesCount();
		virtual void value(Model* model, vis::Polygon* m);
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYGONVERTICESCOUNT_H
