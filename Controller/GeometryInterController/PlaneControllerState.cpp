#include "Controller/GeometryInterController/PlaneControllerState.h"
#include "Rendering/Renderer.h"

PlaneControllerState::PlaneControllerState(){
	speedMov = 0.5;
	changeSpeedMovement = false;
	idPlane = 0;
}

PlaneControllerState::~PlaneControllerState(){
}

void PlaneControllerState::init(){
	ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
    if(configRenderer){
        for (unsigned int i = 0; i < configRenderer->valuePlanes.size(); i++){
            Plane* actualPlane = configRenderer->valuePlanes[i];
            actualPlane->setOrigin(glm::vec3(0.0f));
        }
        updatePlaneInputs(configRenderer);
    }
}

void PlaneControllerState::end(){

}

 
void PlaneControllerState::handleKeyPressEvent(QKeyEvent* event){

	if ( !event->isAutoRepeat() ) {
		context->keyboardState.keyPressed(event->key());
		std::cout << "key event in board: " << event->key() << std::endl;
	}
	if(event->key()==Qt::Key_H){
		// Return to Main model Controller
		getViewer()->drawEditGeometryMode = false;
		context->transitionTo(context->modelController);
		getViewer()->forceReRendering();
		return;
	}
	else if (event->key()==Qt::Key_Q){
		// Enable inputs around Z-axis
		changeSpeedMovement = true;
		getViewer()->setCameraLookingXY();
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_A){
		// Enable inputs around Y-axis
		changeSpeedMovement = true;
		getViewer()->setXRotation(0);
		getViewer()->setYRotation(90*16);
		getViewer()->setZRotation(0); 
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_Z){
		// Enable inputs around X-axis
		changeSpeedMovement = true;
		getViewer()->setXRotation(-90*16);
		getViewer()->setYRotation(0);
		getViewer()->setZRotation(0);
		getViewer()->forceReRendering();
		return;
	}
	else if(event->key()==Qt::Key_W){
		// Change the actual plane to be modfify with user's inputs
		ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
		idPlane = (idPlane + 1) % configRenderer->valuePlanes.size();
	}
}

void PlaneControllerState::handleKeyReleaseEvent(QKeyEvent* event){
	if (!event->isAutoRepeat() ) {
		std::cout << "key event release in board: " << event->key() << std::endl;
		context->keyboardState.keyRelesed(event->key());
	}
	if(event->key()==Qt::Key_Q || event->key()==Qt::Key_A || event->key()==Qt::Key_Z){
		// Disable user's inputs on Planes 
		changeSpeedMovement = false;
		return;
	}
}

void PlaneControllerState::handleMouseReleaseEvent(QMouseEvent* event){
	Q_UNUSED(event);
}

void PlaneControllerState::handleMousePressEvent(QMouseEvent* event){
	// Update mouse's last position
	context->lastPos = event->pos();
}

void PlaneControllerState::handleWheelEvent(QWheelEvent* event){
	bool up = event->angleDelta().y() > 0;
	if(changeSpeedMovement){
		ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
		if (context->keyboardState.isKeyPressed(Qt::Key_Q)){
			// Can rotate the normal vector around his Z-axis
			Plane* actualPlane = configRenderer->valuePlanes[idPlane];
			float angle = glm::radians(5.0f);
			if(!up) angle *= -1;
			actualPlane->setNormal(glm::vec3(
				actualPlane->getNormal().x * glm::cos(angle) - actualPlane->getNormal().y * glm::sin(angle),
				actualPlane->getNormal().x * glm::sin(angle) + actualPlane->getNormal().y * glm::cos(angle),
				actualPlane->getNormal().z)
				);
			updatePlaneInputs(configRenderer);
			return;
		}
		if (context->keyboardState.isKeyPressed(Qt::Key_A)){
			// Can rotate the normal vector around his X-axis
			Plane* actualPlane = configRenderer->valuePlanes[idPlane];
			float angle = glm::radians(5.0f);
			if(!up) angle *= -1;
			actualPlane->setNormal(glm::vec3(
				actualPlane->getNormal().x, 
				actualPlane->getNormal().y * glm::cos(angle) - actualPlane->getNormal().z * glm::sin(angle),
				actualPlane->getNormal().y * glm::sin(angle) + actualPlane->getNormal().z * glm::cos(angle))
				);
			updatePlaneInputs(configRenderer);
			return;
		}
		else if (context->keyboardState.isKeyPressed(Qt::Key_Z)){
			// Can rotate the normal vector around his Y-axis
			Plane* actualPlane = configRenderer->valuePlanes[idPlane];
			float angle = glm::radians(5.0f);
			if(!up) angle *= -1;
			actualPlane->setNormal(glm::vec3(
				actualPlane->getNormal().x * glm::cos(angle) + actualPlane->getNormal().z * glm::sin(angle),
				actualPlane->getNormal().y,
				-actualPlane->getNormal().x * glm::sin(angle) + actualPlane->getNormal().z * glm::cos(angle))
				);
			updatePlaneInputs(configRenderer);
			return;
		}
		return;
	}

	// Check if it has to zoom-in or zoom-out
	float zoomIncrement = 1.1;
	if(up)
		getViewer()->zoom *= zoomIncrement;
	else
		getViewer()->zoom /= zoomIncrement;
    getViewer()->forceReRendering();
}

void PlaneControllerState::handleMouseMoveEvent(QMouseEvent* event){
	// Check delta value of mouse position
	int dx = event->x() - context->lastPos.x();
	int dy = event->y() - context->lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
		Plane* actualPlane = configRenderer->valuePlanes[idPlane];
		if(context->keyboardState.isKeyPressed(Qt::Key_Control)){
			// Change position of main model
			getViewer()->tra += glm::vec3(dx,-dy,0) * glm::vec3(this->context->getActualValueMod());
			getViewer()->needsRefreshDrawing = true;
		}
        else if (context->keyboardState.isKeyPressed(Qt::Key_Q)){
            // Can move sphere geometry (change center)            
            actualPlane->setOrigin(actualPlane->getOrigin() + glm::vec3(dx,-dy,0) * glm::vec3(this->context->getActualValueMod()));
			updatePlaneInputs(configRenderer);
        }
        else if (context->keyboardState.isKeyPressed(Qt::Key_A)){
            // Can move sphere geometry (change center)            
			actualPlane->setOrigin(actualPlane->getOrigin() + glm::vec3(0,-dy,dx) * glm::vec3(this->context->getActualValueMod()));
			updatePlaneInputs(configRenderer);
        }
        else if (context->keyboardState.isKeyPressed(Qt::Key_Z)){
            // Can move sphere geometry (change center)            
			actualPlane->setOrigin(actualPlane->getOrigin() + glm::vec3(dx,0,-dy) * glm::vec3(this->context->getActualValueMod()));
			updatePlaneInputs(configRenderer);
        }
		else {
			// Rotate main model
			getViewer()->setXRotation(getViewer()->rot.x + 8 * dy);
			getViewer()->setYRotation(getViewer()->rot.y + 8 * dx);
		}

	} else if (event->buttons() & Qt::RightButton) {
		// Rotate main model
		getViewer()->setXRotation(getViewer()->rot.x + 8 * dy);
		getViewer()->setZRotation(getViewer()->rot.z + 8 * dx);
	}
	if(getViewer()->needsRefreshDrawing){
		getViewer()->forceReRendering();
		getViewer()->needsRefreshDrawing = false;
	}

	// Update mouse's last position
	context->lastPos = event->pos();
}

void PlaneControllerState::updatePlaneInputs(ConvexGeometryIntersectionRendererConfig* config){
	config->updatePlaneInputs();
	getViewer()->applyRendererConfigChanges();
	getViewer()->needsRefreshDrawing = true;
}