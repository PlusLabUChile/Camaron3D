#ifndef CAMERACAMARON_H
#define CAMERACAMARON_H

#include <QWidget>
#include <glm/glm.hpp>
#include "Utils/matrixtransformationutils.h"
#include <iostream>
#include "Common/Constants.h"

/**
 * @brief Implementation of Camera
 * 
 * This class calculate and store parameters and values to simulate a camera when a model is moved, rotate or zoom-in/zoom-out
 * 
 * 
*/

class CameraCamaron : public QWidget {
    public:
        CameraCamaron();
        ~CameraCamaron();

        void setRotation(glm::vec3);
        void setTraslation(glm::vec3);
        void setViewPortSize(glm::vec2, glm::vec2);
        void setScale(float);
        void setUseOrthoProjection(bool);
        void setPerspectiveValue(float);
        void resetCameraPositions();
        void setModelToCameraSpaceMatrix(glm::mat4);

        glm::mat4 getMV();
        glm::mat4 getMVP();
        glm::mat4 getMVPVP();
        glm::mat3 getNormalMatrix();
        glm::mat4 getViewPortMatrix();
        glm::mat4 getOrthoProjectionMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getPerspectiveProjectionMatrix();
		glm::mat4 getModelToCameraSpaceMatrix();
		glm::mat4 getModelToCameraSpaceAndScaleMatrix();
		glm::vec2 getViewPortSize();

        glm::vec3 getCameraPosition();

		bool isOrthoProjectionInUse();
		void setViewMatrix(glm::mat4);

        glm::vec2 getScreenPosition(glm::vec3);
        
    private:
        // Camera Position
        glm::vec3 positionCamera;

        // Model values
        glm::vec3 traslation;
        glm::vec3 rotation;
        float scale;

        // Model View Projection values
		glm::mat4 MV; //Model View
		glm::mat4 MVP; //Model View Projection
		glm::mat4 MVPVP; //Model View Projection ViewPort

        // ViewPort
        glm::vec2 viewPortSize;
		glm::vec2 viewPortZ;
        glm::mat4 viewportMatrix;
		glm::mat4 viewMatrix;

        // Projections
		glm::mat4 orthoProjectionMatrix;
		glm::mat4 perspectiveProjectionMatrix;
		glm::mat3 normalMatrix;
		float perspectiveValue;

        // Model Space (Bounds of model)
		glm::mat4 modelToCameraSpace;
		glm::mat4 modelToCameraScaleSpace;

        // conditions to compute values
		bool recalculateMVP;
		bool recalculateMV;
		bool recalculateMVPVP;
		bool recalculateNormal;
		bool recalculateModelCameraSpaceScale;
		bool useOrthoProjection;
};

#endif // CAMERACAMARON_H
