#ifndef POLYHEDRONPOLYGONSCOUNT_H
#define POLYHEDRONPOLYGONSCOUNT_H

#include "Model/Model.h"

#include "EvaluationStrategies/PolyhedronEvaluation.h"

class PolyhedronPolygonsCount: public PolyhedronEvaluation
{
	public:
		PolyhedronPolygonsCount();
		virtual ~PolyhedronPolygonsCount();
		virtual void value(Model* model, vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYHEDRONPOLYGONSCOUNT_H
