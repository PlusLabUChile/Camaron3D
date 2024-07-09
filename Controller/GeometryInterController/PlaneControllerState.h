#pragma once
#include "Controller/ControllerState.h"
#include "Controller/ControllerContext.h"
#include "Rendering/Renderers/ConvexGeometryIntersectionRenderer/convexgeometryintersectionrendererconfig.h"

/**
 *  @brief Controller for Sphere Geometry Intersecion on Convex Geometry Renderer.
 *
 * This class will modify parameters in SpereGeometry class used in Geometry Renderer class like the position or 
 * radius of the sphere and the numbers of vertices that Camaron will use to render it.
 *
 * A summary of main controller inputs
 * Keys:
 *   Rotate the camera and enable
 *
 * Mouse:
 *   Rotate the model pressing right-click and the wheel to zoom-in/zoom-out.
 *   Also, using left-click to use MouseSelection Strategy
 * 
*/

class PlaneControllerState : public ControllerState {
    public:
        PlaneControllerState();
        virtual ~PlaneControllerState();
    
        // Methods that need to be overriding for this controller context
        virtual void handleKeyPressEvent(QKeyEvent*);
        virtual void handleKeyReleaseEvent(QKeyEvent*);
        virtual void handleMouseReleaseEvent(QMouseEvent*);
        virtual void handleMousePressEvent(QMouseEvent*);
        virtual void handleWheelEvent(QWheelEvent*);
        virtual void handleMouseMoveEvent(QMouseEvent*);

        /**
         * @brief Update Plane inputs values inside Plane Geometry Widget's form
         * 
         * If the user, usign this inputs context, change a value of a plane, this class
         * will be called. This class will change the values inside the Plane Geometry widget
         * form to new values.
         * 
         * @param ConvexGeometryIntersectionRendererConfig QtWidget that has Plane Geometry form
         * 
        */
        void updatePlaneInputs(ConvexGeometryIntersectionRendererConfig*);

    private:
        bool changeSpeedMovement;
        float speedMov;
        int idPlane;
};