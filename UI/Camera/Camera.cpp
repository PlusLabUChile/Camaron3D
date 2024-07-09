#include "UI/Camera/Camera.h"
#include <iostream>

CameraCamaron::CameraCamaron():
	positionCamera(
		CAMERA_POSITION_X,
		CAMERA_POSITION_Y,
		CAMERA_POSITION_Z
	)	
{
	scale = 1.0f;
	recalculateMVP = true;
	recalculateMV = true;
	recalculateMVPVP = true;
	useOrthoProjection = true;
	recalculateNormal = true;
	recalculateModelCameraSpaceScale = true;
	perspectiveValue = 5.0f;
	viewPortSize = glm::vec2(150.0f, 150.0f);
	viewPortZ = glm::vec2(-150.0f, 150.0f);
	modelToCameraSpace = glm::mat4(1.0f);
};

CameraCamaron::~CameraCamaron(){};

void CameraCamaron::resetCameraPositions(){
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	traslation = glm::vec3(0.0f,0.0f,50.0f);
	scale = 1.0f;
}

glm::vec3 CameraCamaron::getCameraPosition(){
	return positionCamera;
}

void CameraCamaron::setRotation(glm::vec3 vector){
    rotation = vector;
    recalculateMV = true;
    recalculateMVP = true;
    recalculateMVPVP = true;
    recalculateNormal = true;
}

void CameraCamaron::setTraslation(glm::vec3 vector){
    traslation = vector;
    recalculateMV = true;
    recalculateMVP = true;
    recalculateMVPVP = true;
    recalculateNormal = true;
}

void CameraCamaron::setViewPortSize(glm::vec2 rec, glm::vec2 z){
    viewPortSize = rec;
    viewPortZ = z;
    viewportMatrix = MatrixTransformationUtils::generateViewPortMatrix(0,0,rec.x, rec.y, z.x, z.y);
    float ratio = ((float)rec.x)/((float)rec.y);
    if(ratio>1.0f)
        orthoProjectionMatrix = MatrixTransformationUtils::generateOrthoProjectionMatrix(-0.5*ratio, 0.5*ratio, 0.5, -0.5, z.y, z.x);
    else   
        orthoProjectionMatrix = MatrixTransformationUtils::generateOrthoProjectionMatrix(-0.5,0.5,0.5*1.0f/ratio,-0.5*1.0f/ratio,z.y, z.x);
    setPerspectiveValue(perspectiveValue);
    recalculateMVP = true;
    recalculateMVPVP = true;
}

void CameraCamaron::setScale(float value){
    scale = value;
    recalculateModelCameraSpaceScale = true;
	recalculateMV = true;
	recalculateMVP = true;
	recalculateMVPVP = true;
}

void CameraCamaron::setUseOrthoProjection(bool b){
	useOrthoProjection = b;
	recalculateMVP = true;
	recalculateMVPVP = true;
	recalculateModelCameraSpaceScale = true;
}

void CameraCamaron::setPerspectiveValue(float newVal){
	perspectiveValue = newVal;
	float ratio = ((float)viewPortSize.x)/((float)viewPortSize.y);
	float frustum = MatrixTransformationUtils::CalcFrustumScale(perspectiveValue);
	float frustumRatio = MatrixTransformationUtils::CalcFrustumScale(perspectiveValue*ratio);
	perspectiveProjectionMatrix = MatrixTransformationUtils::generatePerspectiveProjectionMatrix(frustumRatio,frustum,
																								 viewPortZ.y, viewPortZ.x,
																								 0.5,-0.5,
																								 0.5*ratio,-0.5*ratio);
}

glm::mat4 CameraCamaron::getMV(){
	if(recalculateMV){
		glm::mat4 value =  MatrixTransformationUtils::generateScaleMatrix(scale)*
						   MatrixTransformationUtils::generateTranslationMatrix(traslation)*
						   MatrixTransformationUtils::generateRotationMatrix(rotation)*
						   getModelToCameraSpaceMatrix();
		MV = value;
		recalculateMV = false;
		recalculateMVP = true;
		recalculateMVPVP = true;
	}
	return MV;
}

glm::mat4 CameraCamaron::getMVP(){
	if(recalculateMVP){
		MVP = getProjectionMatrix()*getMV();
		recalculateMVP = false;
		recalculateMVPVP = true;
	}
	return MVP;
}

glm::mat4 CameraCamaron::getMVPVP(){
	if(recalculateMVPVP){
		MVPVP = getViewPortMatrix()*getMVP();
		recalculateMVPVP = false;
	}
	return MVPVP;
}

glm::mat3 CameraCamaron::getNormalMatrix(){
	if(recalculateNormal){
		normalMatrix = glm::mat3(MatrixTransformationUtils::generateRotationMatrix(rotation));
		recalculateNormal = false;
	}
	return normalMatrix;
}

glm::mat4 CameraCamaron::getViewPortMatrix(){
	return viewportMatrix;
}

glm::mat4 CameraCamaron::getOrthoProjectionMatrix(){
	return orthoProjectionMatrix;
}

glm::mat4 CameraCamaron::getProjectionMatrix(){
	if(useOrthoProjection)
		return getOrthoProjectionMatrix();
	return getPerspectiveProjectionMatrix();
}

glm::mat4 CameraCamaron::getPerspectiveProjectionMatrix(){
	return perspectiveProjectionMatrix;
}

glm::mat4 CameraCamaron::getModelToCameraSpaceMatrix(){
	return modelToCameraSpace;
}

void CameraCamaron::setModelToCameraSpaceMatrix(glm::mat4 matrix){
	modelToCameraSpace = matrix;
}

glm::mat4 CameraCamaron::getModelToCameraSpaceAndScaleMatrix(){
	if(recalculateModelCameraSpaceScale){
		recalculateModelCameraSpaceScale = false;
		modelToCameraScaleSpace = MatrixTransformationUtils::generateScaleMatrix(scale)*
								  getModelToCameraSpaceMatrix();
	}
	return modelToCameraScaleSpace;
}

glm::vec2 CameraCamaron::getViewPortSize(){
	return viewPortSize;
}

bool CameraCamaron::isOrthoProjectionInUse(){
	return useOrthoProjection;
}

void CameraCamaron::setViewMatrix(glm::mat4 m){
	viewMatrix = m;
}

glm::vec2 CameraCamaron::getScreenPosition(glm::vec3 position3D){
	glm::vec3 vpoint = getMVP() * glm::vec4(position3D, 1.0f);
	glm::vec2 screenPoint = glm::vec2(0.0f);
	screenPoint.x = viewPortSize.x * vpoint.x / 2.0f + (viewPortSize.x/2.0f);
	screenPoint.y = viewPortSize.y * vpoint.y / 2.0f + (viewPortSize.y/2.0f);
	screenPoint.y = viewPortSize.y - screenPoint.y;
	return screenPoint;
}