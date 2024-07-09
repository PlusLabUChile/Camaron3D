#ifndef UNITCUBE_H
#define UNITCUBE_H

#include <vector>

namespace vis{
class Vertex;
class Edge;
class Polygon;
class Polyhedron;
}

class Model;

class UnitCube {
	public:
		UnitCube(bool tri);
		~UnitCube();

		// Meshes
		Model* vertexcloud;
		Model* polygonmesh;
		Model* polyhedronmesh;

		void setAdditionalEdges();

	private:
		bool triangulate;
		void loadVertices(Model*);
		void loadEdges(Model*);
		void loadTriangles(Model*);
		void loadSquares(Model*);
		///@note: It might be interesting to use other tetrahedron configurations,
		/// but will need extra work to do so.
		///@see: https://www.ics.uci.edu/~eppstein/projects/tetra/
		void loadTetrahedrons(Model*);
		void loadHexahedron(Model*);
		void createVertexCloud();
		void createPolygonMesh();
		void createPolyhedronMesh();

		void setBounds(Model* mesh);
};

#endif