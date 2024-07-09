#ifndef RENDERELEMENTSIDS_H
#define RENDERELEMENTSIDS_H

#include <GL/glew.h>
#include <QWidget>
#include "Utils/dynamicorderedcontainerwithmaxsize.h"
#include "UI/genericconfigwidget.h"
#include "UI/Camera/Camera.h"

class Renderer;
class RModel;
class Model;
class Selection;
namespace vis{
class Element;
}
namespace Ui {
class RenderElementsIds;
}

class RenderElementsIds : public QWidget, public GenericConfigWidget
{
		Q_OBJECT
		
	public:
		explicit RenderElementsIds(QWidget *parent = 0);
		~RenderElementsIds();
		void renderIds(Renderer*, RModel*, Selection* , QPainter& painter, CameraCamaron*);
		void setElementsType(int);
		void initializeShader();
		void readGenericConfig();
		QWidget* getGenericWidget();

	private:
		void renderIdsUsingShaders(Renderer*, RModel*,CameraCamaron*);
		void renderVertexIds(Renderer*, RModel*, QPainter& painter, Model*, CameraCamaron*);
		void renderPolygonIds(Renderer*, RModel*, QPainter& painter, Model*, CameraCamaron*);
		void renderPolyhedronIds(Renderer*, RModel*, QPainter& painter, Model*, CameraCamaron*);
		void renderOrderedElementsIds(QPainter& painter);
		void processElement(vis::Element* e,Renderer*, RModel*, bool isAtSurface, bool ignoreSelection, CameraCamaron*);
		bool discard(bool ignoreSelection = false, bool isAtSurface = false);
		bool discardSelection(vis::Element*);
		void readConfig();
		Ui::RenderElementsIds *ui;
		int elementsType;
		DynamicOrderedContainerWithMaxSize container;
		int elementIdDrawnOption;
		static const int DRAW_ONLY_SURFACE = 0;
		static const int DRAW_ONLY_INTERIOR = 1;
		static const int DRAW_ONLY_SELECTED = 2;
		static const int DRAW_ALL = 3;
		int elementIdDrawnOptionFacing;
		static const int DRAW_FRONT_FACES = 0;
		static const int DRAW_BACK_FACES = 1;
		static const int DRAW_ALL_FACES = 2;
		static const int DRAW_CLOSER_THAN = 3;
		float maxDistance;
		float fontSize;
		glm::vec4 textColor;
		glm::vec4 textFillColor;
		glm::vec3 viewPortCenter;
		bool useDepthStrategy; //false = distance Strategy
		unsigned int nElements; //false = distance Strategy
		//Shader Acceleration
		bool useShaderAcceleration;
		bool shaderOk;
		GLuint theProgram;
		GLuint textureId;
		bool compileShaderProgram();
		bool loadNumberTexture();
};

#endif // RENDERELEMENTSIDS_H
