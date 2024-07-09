#ifndef POLYGONAREA_H
#define POLYGONAREA_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"

class PolygonArea: public PolygonEvaluation
{
	public:
		PolygonArea();
		virtual ~PolygonArea();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // AREA_H
