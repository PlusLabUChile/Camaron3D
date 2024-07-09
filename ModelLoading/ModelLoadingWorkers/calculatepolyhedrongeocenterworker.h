#ifndef CALCULATEPOLYHEDRONGEOCENTERWORKER_H
#define CALCULATEPOLYHEDRONGEOCENTERWORKER_H

#include "Utils/qtworkerbase.h"
class Model;
class CalculatePolyhedronGeocenterWorker: public QtWorkerBase
{
	public:
		CalculatePolyhedronGeocenterWorker(Model* mesh,
										   unsigned int min,
										   unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // CALCULATEPOLYHEDRONGEOCENTERWORKER_H
