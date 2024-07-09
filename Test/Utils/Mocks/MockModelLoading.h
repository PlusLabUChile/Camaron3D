#ifndef MOCKMODELLOADING_H
#define MOCKMODELLOADING_H

class Model;
class MockModelLoading {
	public:
		MockModelLoading();
		~MockModelLoading();

		//Sequential versions of each step for benchmarking purposes
		void SEQcompleteVertexPolygonRelations(Model* mesh);
		void SEQcompletePolygonPolygonRelations(Model* mesh);
		void SEQcompletePolygonPolyhedronRelations(Model* mesh);
		void SEQcalculateNormalsPolygons(Model* mesh);
		void SEQcalculateNormalsVertices(Model* mesh);
		void SEQcalculateGeoCenterPolyhedronMesh(Model* mesh);

	private:
		};

#endif