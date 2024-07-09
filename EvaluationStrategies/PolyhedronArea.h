#ifndef POLYHEDRONAREA_H
#define POLYHEDRONAREA_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolyhedronEvaluation.h"

class PolyhedronArea: public PolyhedronEvaluation
{
	public:
		PolyhedronArea();
		virtual ~PolyhedronArea();
		virtual void value(Model* model, vis::Polyhedron* m );
		virtual float getNullValue();
		void QApplicationInitiatedEv();
	private:
};

#endif // POLYHEDRONAREA_H
