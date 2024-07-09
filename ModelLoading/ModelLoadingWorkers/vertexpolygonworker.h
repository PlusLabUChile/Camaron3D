#ifndef VERTEXPOLYGONWORKER_H
#define VERTEXPOLYGONWORKER_H
#include "Utils/qtworkerbase.h"
class Model;
class VertexPolygonWorker: public QtWorkerBase
{
	public:
		VertexPolygonWorker(Model* mesh,
										 unsigned int min,
										 unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // VERTEXPOLYGONWORKER_H
