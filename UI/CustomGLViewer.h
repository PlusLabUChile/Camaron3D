#ifndef CUSTOMGLVIEWER_H
#define CUSTOMGLVIEWER_H

#include "UI/RenderElementsIds/renderelementsids.h"
#include "UI/mainconfiguration.h"
#include "UI/rendererslist.h"
#include <QTimer>
#include <QGLWidget>
#include <glm/glm.hpp>
#include "Utils/keyboardstate.h"
#include "Utils/renderinglock.h"
#include "UI/Camera/Camera.h"
#include "Controller/ControllerContext.h"
#include "Utils/glps2/gl2ps.h"

class Selection;
class Renderer;
class RModel;
class Model;
class FrameBufferObjectHandle;
class CustomGLViewer: public QGLWidget
{
		Q_OBJECT

	public:
		CustomGLViewer(RModel*,CameraCamaron*,MainConfiguration&,
					   RenderersList&,QWidget *parent = 0);
		~CustomGLViewer();
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		void setSelection(Selection*);
		void refreshRenderedModel();
		void glewInitiated();
		void refreshHelpers();
		void setAxesVisible(bool value);
		void resetCameraPositions();
		void setCameraLookingXY();

		// Reset functions
		void resetController(Model*);
		void reset(Model*);
		void toModelController();

		// Getters Functions
		void setRenderer(Renderer*);
		Renderer* getRenderer();
		CameraCamaron* getCamera();
		RModel* getRModel();
		ControllerContext* getController();

		// Variables
		glm::vec3 rot;
		glm::vec3 tra;        
		float zoom;

		// Variables Mouse Selection
		int dxSelectBox,dySelectBox;
		bool drawBox;

		// Variables Draw element's ID
		bool drawIds;
        int elementTypeIds;
        bool needsRefreshDrawing;
        bool boxSelect;

		// Variables Geometry Intersection
		bool drawEditGeometryMode;

	public slots:
		void applyRendererConfigChanges();
		void processRenderingQueue();
		void forceReRendering();
		void setXRotation(int);
		void setYRotation(int);
		void setZRotation(int);
		void saveRenderedModelAsQImage();

	signals:
		void xRotationChanged(int angle);
		void yRotationChanged(int angle);
		void zRotationChanged(int angle);
		void selectionBox(int,int,int,int);

	protected:
		void initializeGL();
		void paintEvent(QPaintEvent *);
		//void paintGL();
		void resizeGL(int width, int height);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent ( QWheelEvent * event );
		void keyPressEvent(QKeyEvent* event);
		void keyReleaseEvent(QKeyEvent* event);
		QSize minimumSize();
		QSize minimumSizeHint();
		QSize sizeHint();

	private:
//		QtLogo *logo;
		CameraCamaron* camera;
		glm::vec2 viewportSize;
		float lastFps;
		//from parent
		Renderer* renderer;
		RenderersList& secondaryRenderers;
		RModel* rmodel;
		Selection* selection;
		//CONFIG
		MainConfiguration& mainConfiguration;

		// Controller
		ControllerContext* controller;

		QImage screenshot;
		bool glewInitiatedState;
		RenderingLock renderingLock;
		QTimer renderingTimer;
		RenderElementsIds *renderElementsIds;
		void paintOpenGl3DScene();
		void paintUsingRenderers();
		void paintSelectBoxOverlay(QPainter &painter);
		void paintScreenshotAsBackground(QPainter &painter);
		void paintFrameRateDisplay(QPainter &painter,float framerate);
		void paintEditConvexGeometryDisplay(QPainter &painter);

		//Average OIT
		GLuint weightedAverageProgram;
		FrameBufferObjectHandle* averageOITfboHandle;
		GLuint averageOITdepthStencilTextureHandle;
		GLuint averageOITmainTextureHandle;
		GLuint averageOITbackgroundTextureHandle;
		void initWeightedAverageShader();
		void resizeOITFBOTextures();
		const static GLuint VERTEX_POSITION = 0;
};
#endif // CUSTOMGLVIEWER_H
