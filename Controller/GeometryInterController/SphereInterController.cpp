#include "Controller/GeometryInterController/SphereInterController.h"
#include "Rendering/Renderer.h"

SphereInterController::SphereInterController(){
	speedMov = 0.5;
	changeSpeedMovement = false;
}

SphereInterController::~SphereInterController(){
}

void SphereInterController::init(){
	ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
    if(configRenderer){
        configRenderer->radiusSphere = this->context->getMaxLengthModel();
        configRenderer->centerSphere = glm::vec3(0.0f);
        updateSphereInputs(configRenderer);
    }
}

void SphereInterController::end(){

}

 
void SphereInterController::handleKeyPressEvent(QKeyEvent* event){
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
	else if(event->key()==Qt::Key_Q){
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
	else if(event->key()==Qt::Key_Space){
		// Reset Sphere Geometry values
		ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
		configRenderer->centerSphere = glm::vec3(0, 0, 0);
		updateSphereInputs(configRenderer);
	}
	
}

void SphereInterController::handleKeyReleaseEvent(QKeyEvent* event){
	if (!event->isAutoRepeat() ) {
		std::cout << "key event release in board: " << event->key() << std::endl;
		context->keyboardState.keyRelesed(event->key());
	}
	if(event->key()==Qt::Key_Q || event->key()==Qt::Key_A || event->key()==Qt::Key_Z){
		changeSpeedMovement = false;
	}
}

void SphereInterController::handleMouseReleaseEvent(QMouseEvent* event){
	Q_UNUSED(event);
}

void SphereInterController::handleMousePressEvent(QMouseEvent* event){
	context->lastPos = event->pos();
}

void SphereInterController::handleWheelEvent(QWheelEvent* event){
	bool up = event->angleDelta().y() > 0;
	if(changeSpeedMovement){
		if(up) speedMov = std::max(speedMov + 0.1f, 1.0f);
		else speedMov = std::max(speedMov - 0.1f, 0.1f);
		return;
	}
	float zoomIncrement = 1.1;
	if(up)
		getViewer()->zoom *= zoomIncrement;
	else
		getViewer()->zoom /= zoomIncrement;
    getViewer()->forceReRendering();
}

void SphereInterController::handleMouseMoveEvent(QMouseEvent* event){
	int dx = event->x() - context->lastPos.x();
	int dy = event->y() - context->lastPos.y();
	if (event->buttons() & Qt::LeftButton) {
		ConvexGeometryIntersectionRendererConfig* configRenderer = (ConvexGeometryIntersectionRendererConfig*) getViewer()->getRenderer()->getRendererConfigWidget();
		if(context->keyboardState.isKeyPressed(Qt::Key_Control)){
			// Change position of main model.
			getViewer()->tra += glm::vec3(dx,-dy,0) * glm::vec3(this->context->getActualValueMod());
			getViewer()->needsRefreshDrawing = true;
		}
        else if (context->keyboardState.isKeyPressed(Qt::Key_Q)){
            // Can move sphere geometry (change center)  .          
            configRenderer->centerSphere += glm::vec3(dx,-dy,0) * glm::vec3(this->context->getActualValueMod());
			updateSphereInputs(configRenderer);
        }
        else if (context->keyboardState.isKeyPressed(Qt::Key_A)){
            // Can move sphere geometry (change center) .           
            configRenderer->centerSphere += glm::vec3(0,-dy,dx) * glm::vec3(this->context->getActualValueMod());
			updateSphereInputs(configRenderer);
        }
        else if (context->keyboardState.isKeyPressed(Qt::Key_Z)){
            // Can move sphere geometry (change center)   .         
            configRenderer->centerSphere += glm::vec3(dx,0,-dy) * glm::vec3(this->context->getActualValueMod());
			updateSphereInputs(configRenderer);
        }
        else if (context->keyboardState.isKeyPressed(Qt::Key_W)){
            // Can modify radius value of sphere geometry.
			glm::vec2 sphereScreenPoint = getViewer()->getCamera()->getScreenPosition(configRenderer->centerSphere);
			float dotValue = glm::dot(glm::vec2(context->lastPos.x(), context->lastPos.y()) - sphereScreenPoint, glm::vec2(dx,dy));
			float step = (dotValue > 0) ? this->context->getActualValueMod() * 100 : -this->context->getActualValueMod() * 100;
			configRenderer->radiusSphere += step;
			updateSphereInputs(configRenderer);
        }
		else {
			// Rotate main model Z-axis.
			getViewer()->setXRotation(getViewer()->rot.x + 8 * dy);
			getViewer()->setYRotation(getViewer()->rot.y + 8 * dx);
		}

	} else if (event->buttons() & Qt::RightButton) {
		// Rotate main model around Y-axis.
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

void SphereInterController::updateSphereInputs(ConvexGeometryIntersectionRendererConfig* config){
	config->updateSphereInputs();
	getViewer()->applyRendererConfigChanges();
	getViewer()->needsRefreshDrawing = true;
}
