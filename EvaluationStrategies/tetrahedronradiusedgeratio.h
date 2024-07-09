#ifndef TETRAHEDRONRADIUSEDGERATIO_H
#define TETRAHEDRONRADIUSEDGERATIO_H

#include "Model/Model.h"
#include "EvaluationStrategies/PolyhedronEvaluation.h"
class TetrahedronRadiusEdgeRatio: public PolyhedronEvaluation
{
	public:
		TetrahedronRadiusEdgeRatio();
		virtual ~TetrahedronRadiusEdgeRatio();
		virtual void value(Model* model, vis::Polyhedron* m);
		virtual float getNullValue();
		void QApplicationInitiatedEv();
};

#endif // TETRAHEDRONRADIUSEDGERATIO_H
