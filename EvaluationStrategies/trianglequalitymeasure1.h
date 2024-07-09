#ifndef TRIANGLEQUALITYMEASURE1_H
#define TRIANGLEQUALITYMEASURE1_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolygonEvaluation.h"
class TriangleQualityMeasure1: public PolygonEvaluation
{
	public:
		TriangleQualityMeasure1();
		virtual ~TriangleQualityMeasure1();
		virtual void value(Model* model, vis::Polygon* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // TRIANGLEQUALITYMEASURE1_H
