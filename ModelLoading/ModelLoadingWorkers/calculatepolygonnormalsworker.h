#ifndef CALCULATEPOLYGONNORMALSWORKER_H
#define CALCULATEPOLYGONNORMALSWORKER_H

#include "Utils/qtworkerbase.h"
class Model;
class CalculatePolygonNormalsWorker: public QtWorkerBase
{
	public:
		CalculatePolygonNormalsWorker(Model* mesh,
									  unsigned int min,
									  unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEPOLYGONNORMALSWORKER_H
