#ifndef CALCULATEVERTEXNORMALSWORKER_H
#define CALCULATEVERTEXNORMALSWORKER_H

#include "Utils/qtworkerbase.h"
class Model;
class CalculateVertexNormalsWorker: public QtWorkerBase
{
	public:
		CalculateVertexNormalsWorker(Model* mesh,
									 unsigned int min,
									 unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEVERTEXNORMALSWORKER_H
