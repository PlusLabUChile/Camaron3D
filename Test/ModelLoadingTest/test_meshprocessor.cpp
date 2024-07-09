#include "test_meshprocessor.h"
#include "Test/Utils/unitcube.h"
#include "ModelLoading/MeshProcessor.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"

void MeshProcessorTest::initTestCase(){
	Cube = new UnitCube(false);
	}


void MeshProcessorTest::vertexPolygonRelationship(){
	Model* model = Cube->polyhedronmesh;
	Tested->completeVertexPolygonRelations(model);

	int expectedPolygonIds[8][3] = {{0, 2, 3}, {2, 3, 5}, {0, 3, 4}, {3, 4, 5},
									{0, 1, 2}, {1, 2, 5}, {0, 1, 4}, {1, 4, 5}};

	std::vector<vis::Vertex>& verts = model->getVertices();
	for (int i = 0; i < 8; i++){
		std::vector<vis::Polygon*> faces = verts[i].getVertexPolygons();
		for (int j = 0; j < 3; j++){
			QCOMPARE(expectedPolygonIds[i][j], faces[j]->getId());
			}
		}
	}


void MeshProcessorTest::polygonPolygonRelationship(){
	Model* model = Cube->polyhedronmesh;
	Tested->completePolygonPolygonRelations(model);

	int expectedPolygonIds[6][4] = {{3, 4, 1, 2}, {0, 4, 2, 5}, {0, 1, 3, 5},
									{5, 4, 0, 2}, {5, 1, 0, 3}, {2, 1, 3, 4}};

	std::vector<vis::Polygon>& polys = model->getPolygons();
	for (int i = 0; i < 6; i++){
		std::vector<vis::Polygon*> nearby_faces = polys[i].getNeighborPolygons();
		for (int j = 0; j < 4; j++){
			QCOMPARE(expectedPolygonIds[i][j], nearby_faces[j]->getId());
			}
		}
	}

void MeshProcessorTest::normalPolygons(){
	Model* model = Cube->polyhedronmesh;
	Tested->calculateNormalsPolygons(model);
	glm::vec3 expectedNormals[6] = {glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)),
							glm::normalize(glm::vec3(1.0f, 0.0f,  0.0f)),
							glm::normalize(glm::vec3(0.0f,	-1.0f, 0.0f)),
							glm::normalize(glm::vec3(-1.0f,	 0.0f,	0.0f)),
							glm::normalize(glm::vec3( 0.0f, 1.0f, 0.0f)),
							glm::normalize(glm::vec3( 0.0f, 0.0f,  1.0f)),
							};

	std::vector<vis::Polygon>& polys = model->getPolygons();
	for (int i = 0; i < 6; i++){
		QCOMPARE(expectedNormals[i], polys[i].getNormal());
		}
	}

void MeshProcessorTest::normalVertices(){
	Model* model = Cube->polyhedronmesh;
	Tested->calculateNormalsVertices(model);
	glm::vec3 expectedNormals[8] = {glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
							glm::normalize(glm::vec3(-1.0f, -1.0f,	1.0f)),
							glm::normalize(glm::vec3(-1.0f,	 1.0f, -1.0f)),
							glm::normalize(glm::vec3(-1.0f,	 1.0f,	1.0f)),
							glm::normalize(glm::vec3( 1.0f, -1.0f, -1.0f)),
							glm::normalize(glm::vec3( 1.0f, -1.0f,	1.0f)),
							glm::normalize(glm::vec3( 1.0f,	 1.0f, -1.0f)),
							glm::normalize(glm::vec3( 1.0f,	 1.0f,	1.0f)),
							};


	std::vector<vis::Vertex>& verts = model->getVertices();
	for (int i = 0; i < 8; i++){
		QCOMPARE(expectedNormals[i], verts[i].getNormal());
		}
	}

void MeshProcessorTest::polyhedronGeocenter(){
	QSKIP("We need a more complex model to test this one");
	}

void MeshProcessorTest::fixNormals(){
	QSKIP("Need to determine if this is actually necessary");
	}

void MeshProcessorTest::cleanupTestCase(){
	delete Cube;
	}
