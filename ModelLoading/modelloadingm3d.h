#ifndef MODELLOADINGM3D_H
#define MODELLOADINGM3D_H

#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"
#include "Utils/hashingtree.h"
namespace vis{
class Polygon;
class Vertex;
}
class ModelLoadingM3D: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingM3D)
	private:
		void readVertices(Model*);
		bool readPolyhedrons(Model*);
		bool moveToElements(Model*);

		//read Elements
		void readM3DPolyhedron(int nVertices,
							   std::vector<std::vector<int> >& verticesIndicesPerFace,
							   Model* pol,
							   vis::ut::HashingTree<int,int>& hashingTree,
							   unsigned int& polygonCount);

		StreamScanner parser;
		// Face orientation for each polyhedron type
		std::vector<std::vector<int>> tetrahedronIndices = {{0,2,1}, {1,2,3},
		                                                     {1,3,0}, {0,3,2}};
		std::vector<std::vector<int>> pyramidIndices = {{0,3,2,1}, {1,2,4},
		                                                 {2,3,4}, {3,0,4}, {0,1,4}};
		std::vector<std::vector<int>> prismIndices = {{0,1,4,3}, {1,2,5,4},
												       {0,3,5,2}, {0,2,1}, {3,4,5}};

		std::vector<std::vector<int>> hexahedronIndices = {{0,3,2,1}, {3,7,6,2},
															{4,5,6,7}, {0,1,5,4},
												            {1,2,6,5}, {0,4,7,3}};
};


#endif // MODELLOADINGM3D_H
