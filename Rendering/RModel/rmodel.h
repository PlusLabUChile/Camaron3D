#ifndef RMODEL_H
#define RMODEL_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Rendering/RModel/RVertexFlagAttribute.h"
#include "Rendering/RModel/rmodelpropertyfielddef.h"
#include "UI/Camera/Camera.h"
#include "Common/Constants.h"

class ScalarFieldDef;
class PropertyFieldDef;
class Model;

class RModel
{
	public:
		RModel();
		~RModel();

		void loadRModelData(Model*, CameraCamaron*);

		void loadVertexPolygonPolyhedronIds(Model* mesh);
		void loadVertexPositionAndNormals(Model* model);
		void loadAdditionalEdges(Model* model);
		std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> loadPropertyField(std::shared_ptr<ScalarFieldDef>);
		void loadTetrahedronVertexIds(Model* mesh);

		void refreshVertexAttributes(Model*);
		void refreshSelectedElements();
		void deleteData();

		Model* getOriginalModel();

		std::vector<RVertexFlagAttribute> vertexFlagsAttribute;
		std::vector<RVertexFlagAttribute> kernel_vertexFlagsAttribute;
		std::vector<float> bounds;

		int nVertices = 0;
		int nAdditionalEdges = 0;
		int numberOfTriangles = 0;
		int numberOfTetrahedrons = 0;

		static const GLuint NULL_BUFFER = 0;
		//Main Rendering Model
		GLuint positionDataBufferObject;
		GLuint vertexNormalDataBufferObject;
		GLuint vertexFlagsDataBufferObject;
		GLuint rmodelVertexPositionBufferObject;
		GLuint edgeVertexPositionsDataBufferObject;
		GLuint edgeColorDataBufferObject;
		//ids
		GLuint polygonPolyhedronIdsBufferObject;
		GLuint vertexIdsBufferObject;
		GLuint tetrahedronVertexIdsBufferObject;

		// Kernel Rendering Model
		GLuint kernel_positionDataBufferObject;
		GLuint kernel_vertexFlagsDataBufferObject;
		GLuint kernel_vertexNormalDataBufferObject;
		GLuint kernel_rmodelVertexPositionBufferObject;
		void setKernelBuffers(RModel*);
		void loadKernelRModelData(Model*);
		void freeRAMKernelVideoCardBuffer();
		
		void setBackgroundColor(glm::vec4);
		glm::vec4 getBackgroundColor();
		bool isKernelComputed();
		void setKernelComputed(bool);

	private:
		std::shared_ptr<RModelPropertyFieldDef<PropertyFieldDef>> currentRModelPropertyFieldDef;
		void copyPropertyFieldDefs(Model*);
		void uploadVerticesDirectly(Model*);
		glm::vec4 backgroundColor;
		void copyModelBounds(Model* model);
		void freeRAMFromVideoCardBuffer();
		Model* originalModel;
		bool useOrthoProjection;
		bool kernelComputed;

};

#endif // RMODEL_H
