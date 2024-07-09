#include "test_rmodel.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Common/Constants.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include "Model/Element/Vertex.h"
#include "Model/Model.h"
#include "Test/Utils/unitcube.h"
#include "Test/Utils/openglcontext.h"
#include "Test/Utils/meshextensions.h"
#include "Model/Element/Edge.h"

void RModelTest::initTestCase(){
	context = new OpenGLContext(20, 20);
	try{
		context->setupContext();
		context->setupOpenGL();
		}
	catch (std::runtime_error &error){
		delete context;
		QFAIL(error.what());
		}
	Cube = new UnitCube(false);
	Tested = new RModel();
	}


void RModelTest::loadVertexCloud(){
	Model* model = Cube->vertexcloud;
	Tested->loadRModelData(model);
	QVERIFY(MeshExtensions::compare(Tested->getOriginalModel(), Cube->vertexcloud));
	QCOMPARE(Tested->nVertices, 8);

	/// Verify that the model was placed inside the GPU Buffer Object correctly
	std::vector<glm::vec3> expected_verts;
	expected_verts.reserve(8);
	for(vis::Vertex vertex : model->getVertices()) {
		expected_verts.push_back(vertex.getCoords());
		}

	verifyGPUBuffer<glm::vec3>(Tested->positionDataBufferObject,
					expected_verts);

	///@todo Compare values from flag buffers (Find way to compare RVertexFlagAttributes
	}

void RModelTest::loadAdditionalEdges(){
	Model* model = Cube->polygonmesh;

	// Load edges into model
	std::vector<vis::Edge> &edges = model->getAdditionalEdges();
	std::vector<vis::Vertex> &vertices = model->getVertices();

	// Edge on the surface
	edges.emplace_back(0, &vertices[0], &vertices[5], glm::vec3(0.0f, 1.0f, 2.0f));
	// Edge inside the cube
	edges.emplace_back(1, &vertices[0], &vertices[7], glm::vec3(3.0f, 4.0f, 5.0f));

	Tested->loadAdditionalEdges(model);
	QVERIFY(Tested->nAdditionalEdges == 2);

	/// Verify GPU consistency
	std::vector<glm::vec3> expected_coords = {vertices[0].getCoords(),
											  vertices[5].getCoords(),
											  vertices[0].getCoords(),
											  vertices[7].getCoords()};


	verifyGPUBuffer<glm::vec3>(Tested->edgeVertexPositionsDataBufferObject, expected_coords);
	std::vector<glm::vec3> expected_colors = {glm::vec3(0.0f, 1.0f, 2.0f),
											  glm::vec3(0.0f, 1.0f, 2.0f),
											  glm::vec3(3.0f, 4.0f, 5.0f),
											  glm::vec3(3.0f, 4.0f, 5.0f)};
	verifyGPUBuffer<glm::vec3>(Tested->edgeColorDataBufferObject, expected_colors);
	}

void RModelTest::loadVertexPositionAndNormals(){
	Model* model = Cube->polygonmesh;

	/// Inject the expected normals inside the model before calling the function
	glm::vec3 normals[8] = {glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
							glm::normalize(glm::vec3(-1.0f, -1.0f,	1.0f)),
							glm::normalize(glm::vec3(-1.0f,	 1.0f, -1.0f)),
							glm::normalize(glm::vec3(-1.0f,	 1.0f,	1.0f)),
							glm::normalize(glm::vec3( 1.0f, -1.0f, -1.0f)),
							glm::normalize(glm::vec3( 1.0f, -1.0f,	1.0f)),
							glm::normalize(glm::vec3( 1.0f,	 1.0f, -1.0f)),
							glm::normalize(glm::vec3( 1.0f,	 1.0f,	1.0f)),
							};

	std::vector<vis::Vertex> &vertices = model->getVertices();
	for (int i = 0; i < 8; i++){
		vertices[i].setNormal(normals[i]);
		}
	Tested->loadVertexPositionAndNormals(model);

	/// Verify the data was stored in the GPU properly
	int indeces[36] = {0, 2, 6, 0, 6, 4,
					   4, 6, 7, 4, 7, 5,
					   0, 4, 5, 0, 5, 1,
					   1, 3, 2, 1, 2, 0,
					   3, 7, 6, 3, 6, 2,
					   1, 5, 7, 1, 7, 3};

	std::vector<glm::vec3> expected_verts;
	expected_verts.reserve(36);
	for(int index : indeces) {
		expected_verts.push_back(vertices[index].getCoords());
		}

	verifyGPUBuffer<glm::vec3>(Tested->positionDataBufferObject, expected_verts);

	std::vector<glm::vec3> expected_normals;
	expected_verts.reserve(36);
	for (int index : indeces){
		expected_normals.push_back(normals[index]);
		}

	verifyGPUBuffer<glm::vec3>(Tested->vertexNormalDataBufferObject, expected_normals);
	}

void RModelTest::loadPolygonMesh(){
	Model* model = Cube->polygonmesh;
	Tested->loadRModelData(model);

	QVERIFY(Tested->getOriginalModel() == Cube->polygonmesh);
	QCOMPARE(Tested->nVertices, 36); // 6 faces with 2 triangles with 3 vertices each)

	///@note Most buffer contents are verified in tests "loadElementIds" and "loadVertexPositionAndNormals"

	///@todo Verify that the flags are correctly placed inside the GPU
	// GLuint vertexFlagDBO = Tested->vertexFlagsDataBufferObject;

	}

/// @todo We have tests for the methods that are used by this call, but we must
///		   ensure that the method complies with what is expected of it. Focus on the
///		   rest of the tests before dealing with this one.
void RModelTest::loadPolyhedronMesh(){
	QSKIP("Unimplemented");
}

/***
*  For this test, the method must be able to decompose the test mesh into
*  multiple tetrahedrons, triangulating sides that have more than three vertices
*  in the process.
*
*  @todo: Test the case where the mesh is only made of tetrahedrons
*  @todo: Test more exotic shapes
*/
void RModelTest::loadTetrahedronIds(){
	Model* model = Cube->polyhedronmesh;

	// We must populate the RModel position vector of each square before continuing
	/// @todo: Check if we can do this without depending on this method
	Tested->loadRModelData(model);
	Tested->loadTetrahedronVertexIds(model);

	QVERIFY(Tested->numberOfTetrahedrons == 6);

	// Recreate tetrahedron configuration for the cube data, the index listed
	// are the first instance of a vertex inside the RModel buffer
	std::vector<GLuint> expected_tetra =  {0, 5, 2, 8,
										   0, 5, 8, 11,
										   0, 19, 8, 2,
										   0, 19, 2, 1,
										   0, 17, 11, 8,
										   0, 17, 8, 19};

	verifyGPUBuffer<GLuint>(Tested->tetrahedronVertexIdsBufferObject, expected_tetra);
	}

void RModelTest::loadElementIds(){
	Model* model = Cube->polygonmesh;

	Tested->loadVertexPolygonPolyhedronIds(model);
	std::vector<int> indeces = {0, 2, 6, 0, 6, 4,
								4, 6, 7, 4, 7, 5,
								0, 4, 5, 0, 5, 1,
								1, 3, 2, 1, 2, 0,
								3, 7, 6, 3, 6, 2,
								1, 5, 7, 1, 7, 3};

	verifyGPUBuffer<int>(Tested->vertexIdsBufferObject, indeces);
	// rmodelVertexPositionBufferObject is a sequential int buffer, no need to test its contents
	/// @todo Load a tetrahedron based cube to test polygonPolyhedronIdsBufferObject
	///		   contents properly (With internal polyhedrons to populate the buffer)
	/// @note For polyhedron meshes, this method is called before the tetrahedrization process
	///		   is made, leaving the PPID buffer with the indeces of the original polyhedrons
	}

void RModelTest::loadPropertyField(){
	QSKIP("Unimplemented");
	}

void RModelTest::refreshVertexAttributes(){
	QSKIP("Unimplemented");
	}
void RModelTest::refreshSelectedItems(){
	QSKIP("Unimplemented");
	}
void RModelTest::deleteData(){
	QSKIP("Unimplemented");
	}
void RModelTest::setViewPortSize(){
	QSKIP("Unimplemented");
	}
void RModelTest::setPerspectiveValue(){
	QSKIP("Unimplemented");
	}
void RModelTest::getModelToCameraSpaceAndScaleMatrix(){
	QSKIP("Unimplemented");
	}
void RModelTest::cleanupTestCase(){
	delete Tested;
	delete Cube;
	delete context;
	}
