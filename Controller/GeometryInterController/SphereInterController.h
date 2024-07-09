#pragma once
#include "Controller/ControllerState.h"
#include "Controller/ControllerContext.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"

/**
* @brief Controller for Sphere Geometry Intersecion on Convex Geometry Renderer.
*
* This class will modify parameters in SpereGeometry class used in Geometry Renderer class like the position or 
* radius of the sphere and the numbers of vertices that Camaron will use to render it. SphereInterController
* is a child class of Controller State
*
* A summary of main controller inputs
* Keys:
*   Rotate the camera and enable inputs that modify Sphere values.
*
* Mouse:
*   Rotate the model pressing right-click and the wheel to zoom-in/zoom-out.
*   Also, can modify sphere values like position and radius
* 
*/
class SphereInterController : public ControllerState {
    public:
        SphereInterController();
        virtual ~SphereInterController();
    
        // Methods that need to be overriding for this controller context
        virtual void handleKeyPressEvent(QKeyEvent*);
        virtual void handleKeyReleaseEvent(QKeyEvent*);
        virtual void handleMouseReleaseEvent(QMouseEvent*);
        virtual void handleMousePressEvent(QMouseEvent*);
        virtual void handleWheelEvent(QWheelEvent*);
        virtual void handleMouseMoveEvent(QMouseEvent*);

        /**
        * @brief Update Sphere inputs in SphereGeometryWidget (class that has a form with
        * sphere's values to be draw)
        * 
        * This function will be called when the user change a value of the sphere, so 
        * it will update the values inside SphereGeometryWidget
        * 
        * @param ConvexGeometryIntersectionRendererConfig QWidget class that has the Sphere Geometry Form
        */
        void updateSphereInputs(ConvexGeometryIntersectionRendererConfig*);

    private:
        bool changeSpeedMovement;
        float speedMov;
};