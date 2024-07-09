#ifndef TEST_RMODEL_H
#define TEST_RMODEL_H

#include <QTest>
#include "Utils/shaderutils.h"

class RModel;
class UnitCube;
class OpenGLContext;
class RModelTest: public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void loadVertexCloud();
		void loadAdditionalEdges();
		void loadPolygonMesh();
		void loadPolyhedronMesh();
		void loadElementIds();
		void loadVertexPositionAndNormals();
		void loadTetrahedronIds();
		void refreshVertexAttributes();
		void refreshSelectedItems();
		void loadPropertyField();
		void deleteData();
		void setViewPortSize();
		void setPerspectiveValue();
		void getModelToCameraSpaceAndScaleMatrix();
		void cleanupTestCase();

	private:
		UnitCube* Cube;
		RModel* Tested;
		OpenGLContext* context;

		template <class T>
		void verifyGPUBuffer(GLuint, std::vector<T>&);
	};

template <class T>
void RModelTest::verifyGPUBuffer(GLuint target_buffer, std::vector<T>& expected){
	std::vector<T> buffer;
	buffer.resize(expected.size());

	if (!ShaderUtils::recoverDataBuffer<T>(target_buffer, buffer))
		QFAIL("Unable to recover buffer");

	QCOMPARE(buffer, expected);
}

#endif