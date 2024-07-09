#ifndef POLYGONPOLYGONNEIGHBORHOODWORKER_H
#define POLYGONPOLYGONNEIGHBORHOODWORKER_H
#include "Utils/qtworkerbase.h"
class Model;
class PolygonPolygonNeighborhoodWorker: public QtWorkerBase
{
	public:
		PolygonPolygonNeighborhoodWorker(Model* mesh,
										 unsigned int min,
										 unsigned int max);

		void doWork();
	private:
		Model* pmesh;
		unsigned int rangeMin,rangeMax;
};

#endif // POLYGONPOLYGONNEIGHBORHOODWORKER_H
