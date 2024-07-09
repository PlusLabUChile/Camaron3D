#include "Controller/MainController/ModelControllerState.h"
#include <string.h>
#include "Rendering/Renderer.h"

ModelControllerState::ModelControllerState(){
}

ModelControllerState::~ModelControllerState(){
}
 
void ModelControllerState::handleKeyPressEvent(QKeyEvent* event){
	if ( !event->isAutoRepeat() ) {
		context->keyboardState.keyPressed(event->key());
		std::cout << "key event in board: " << event->key() << std::endl;
	}
	if(event->key()==Qt::Key_I){
		// Show polygons ids.
		getViewer()->elementTypeIds = vis::CONSTANTS::POLYGON;
		getViewer()->drawIds = true;
		getViewer()->forceReRendering();
		return;
	}
	if(event->key()==Qt::Key_O){
		// Show polyhedrons ids.
		getViewer()->elementTypeIds = vis::CONSTANTS::POLYHEDRON;
		getViewer()->drawIds = true;
		getViewer()->forceReRendering();
		return;
	}
	if(event->key()==Qt::Key_U){
		// Show vertices ids.
		getViewer()->elementTypeIds = vis::CONSTANTS::VERTEX;
		getViewer()->drawIds = true;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Down && context->keyboardState.isKeyPressed(Qt::Key_Control)){
		// Translate the model Z-axis adding a unitary value of his position.
		getViewer()->tra.z ++;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Up && context->keyboardState.isKeyPressed(Qt::Key_Control)){
		// Translate the model Z-axis substracting a unitary value of his position.
		getViewer()->tra.z --;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Down){
		// Translate the model Y-axis adding a unitary value of his position.
		getViewer()->tra.y ++;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Up){
		// Translate the model Y-axis substracting a unitary value of his position.
		getViewer()->tra.y --;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Left){
		// Translate the model X-axis adding a unitary value of his position.
		getViewer()->tra.x ++;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Right){
		// Translate the model X-axis substracting a unitary value of his position.
		getViewer()->tra.x --;
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Space){
		// Reset camera position, rotation and scale.
		getViewer()->resetCameraPositions();
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_H && strcmp(getViewer()->getRenderer()->getRendererName(),"Geometry Intersection") == 0){
		// Transition to Edit Convex Geometry Mode changing context's controller state
		if(reinterpret_cast<ConvexGeometryIntersectionRendererConfig*>(getViewer()->getRenderer()->getRendererConfigWidget())->actualIndexTab == 0){
			context->transitionTo(context->sphereController);			
		}
		else{
			context->transitionTo(context->planeController);
		}
		getViewer()->drawEditGeometryMode = true;
		getViewer()->forceReRendering();
		return;
	}
}

void ModelControllerState::handleKeyReleaseEvent(QKeyEvent* event){
	if (!event->isAutoRepeat() ) {
		std::cout << "key event release in board: " << event->key() << std::endl;
		context->keyboardState.keyRelesed(event->key());
	}
	if(event->key()==Qt::Key_I || event->key()==Qt::Key_O || event->key()==Qt::Key_U){
		// Undraw the ids of model's elements
		getViewer()->drawIds = false;
		getViewer()->forceReRendering();
	}
}

void ModelControllerState::handleMouseReleaseEvent(QMouseEvent* event){
	Q_UNUSED(event);
	if(getViewer()->drawBox || getViewer()->boxSelect){
		// It calls selectionBox in Mouse Selection Strategy
		getViewer()->drawBox= false;
		getViewer()->boxSelect = false;
		emit getViewer()->selectionBox(
            context->lastPos.x(),
            context->lastPos.y(), 
            context->lastPos.x()+getViewer()->dxSelectBox, 
            context->lastPos.y()+getViewer()->dySelectBox);
        getViewer()->forceReRendering();
	}
}

void ModelControllerState::handleMousePressEvent(QMouseEvent* event){
	// Update mouse position
	context->lastPos = event->pos();
	getViewer()->dxSelectBox = 0;
	getViewer()->dySelectBox = 0;

	if(context->keyboardState.isKeyPressed(Qt::Key_Shift)){
		// Unused screenshot code
		getViewer()->boxSelect = true;
		// getViewer()->saveRenderedModelAsQImage();
	}
	else
		getViewer()->boxSelect = false;
}

void ModelControllerState::handleWheelEvent(QWheelEvent* event){
	// Calculate the wheel input
	bool up = event->angleDelta().y() > 0;
	float zoomIncrement = 1.1;

	// Change zoom value
	if(up)
		getViewer()->zoom *= zoomIncrement;
	else
		getViewer()->zoom /= zoomIncrement;
    getViewer()->forceReRendering();
}

void ModelControllerState::handleMouseMoveEvent(QMouseEvent* event){
	int dx = event->x() - context->lastPos.x();
	int dy = event->y() - context->lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		if(context->keyboardState.isKeyPressed(Qt::Key_Control)){
			// Traslate model
			getViewer()->tra += glm::vec3(dx,-dy,0) * glm::vec3(0.1);
			getViewer()->forceReRendering();
		}
		else {
			if(getViewer()->boxSelect){
				// Check selection box
				getViewer()->drawBox = true;
				getViewer()->dxSelectBox = dx;
				getViewer()->dySelectBox = dy;
				getViewer()->forceReRendering();
				return;
			}
			// Rotate model aroung Z-axis
			getViewer()->setXRotation(getViewer()->rot.x + 8 * dy);
			getViewer()->setYRotation(getViewer()->rot.y + 8 * dx);
		}

	} else if (event->buttons() & Qt::RightButton) {
		// Rotate model around Y-axis
		getViewer()->setXRotation(getViewer()->rot.x + 8 * dy);
		getViewer()->setZRotation(getViewer()->rot.z + 8 * dx);
	}
	if(getViewer()->needsRefreshDrawing){
		getViewer()->forceReRendering();
		getViewer()->needsRefreshDrawing = false;
	}

	// Update last mouse's position
	context->lastPos = event->pos();
}
