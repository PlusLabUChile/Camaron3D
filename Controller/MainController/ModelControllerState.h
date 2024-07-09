#pragma once
#include "Controller/ControllerState.h"
#include "Controller/ControllerContext.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"


/**
* @brief Main Controller for Camaron
*
* This class will modify parameters in classes like CameraCamaron or CustomGLViewer that changes how 
* the model is draw (position, rotation, zoom, display index of elements) and inputs for MouseSelection
* strategy
*
* A summary of main controller inputs
* Keys:
*   Show ids of elements or move Model using arrow keys or with control using the mouse.
*
* Mouse:
*   Rotate the model pressing right-click and the wheel to zoom-in/zoom-out.
*   Also, using left-click to use MouseSelection Strategy
* 
*/
class ModelControllerState : public ControllerState {
    public:
        ModelControllerState();
        virtual ~ModelControllerState();
    
        // Methods that need to be overriding for this controller context
        virtual void handleKeyPressEvent(QKeyEvent*);
        virtual void handleKeyReleaseEvent(QKeyEvent*);
        virtual void handleMouseReleaseEvent(QMouseEvent*);
        virtual void handleMousePressEvent(QMouseEvent*);
        virtual void handleWheelEvent(QWheelEvent*);
        virtual void handleMouseMoveEvent(QMouseEvent*);
};