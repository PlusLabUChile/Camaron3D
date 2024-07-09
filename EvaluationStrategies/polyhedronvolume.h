#ifndef POLYHEDRONVOLUME_H
#define POLYHEDRONVOLUME_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolyhedronEvaluation.h"
class PolyhedronVolume: public PolyhedronEvaluation
{
	public:
		PolyhedronVolume();
		~PolyhedronVolume();
		virtual void value(Model* model, vis::Polyhedron* m);
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // POLYHEDRONVOLUME_H
