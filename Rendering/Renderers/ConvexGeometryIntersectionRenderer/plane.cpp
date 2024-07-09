#include "plane.h"
#include <iostream>


Plane::Plane(glm::vec3 norigin, glm::vec3 v1,glm::vec3 v2):
	normal(glm::cross(v1,v2)),
	origin(norigin),
	planeEquation(normal)
{
	d = glm::dot(origin,normal);
}
Plane::Plane(glm::vec3 norigin, glm::vec3 nnormal):
	normal(nnormal),
	origin(norigin),
	planeEquation(nnormal)
{
	d = glm::dot(origin,normal);
	getPointsPlane();
}

Plane::~Plane(){

}

glm::vec3 Plane::getPointOfIntersection(Plane& p2,Plane& p3){
	glm::mat3 eqSystem(planeEquation,p2.getPlaneEquation(),p3.getPlaneEquation());
	glm::mat3 eqSystemInverse = glm::inverse(eqSystem);
	glm::vec3 dVector(d,p2.getPlaneEquationConstant(),p3.getPlaneEquationConstant());
	return eqSystemInverse*dVector;
}

bool Plane::isOk(){
	return ok;
}
void Plane::fixNormal(glm::vec3 p){
	if(glm::dot(p,normal)<0.0f){
		normal = -normal;
		planeEquation = glm::vec3(normal.x,normal.y,normal.z);
		d = -d;
	}
}

bool Plane::getPointsPlane(){
	if(planeEquation.z != 0){
		pointsPlane.push_back(getPointsWithValues(-500 + origin.x,500 + origin.y,0));
		pointsPlane.push_back(getPointsWithValues(-500 + origin.x,-500 + origin.y,0));
		pointsPlane.push_back(getPointsWithValues(500 + origin.x,500 + origin.y,0));
		pointsPlane.push_back(getPointsWithValues(500 + origin.x,-500 + origin.y,0));
		return true;
	}
	else if(planeEquation.y != 0){
		pointsPlane.push_back(getPointsWithValues(-500 + origin.x, 0, 500 + origin.y));
		pointsPlane.push_back(getPointsWithValues(-500 + origin.x, 0, -500 + origin.y));
		pointsPlane.push_back(getPointsWithValues(500 + origin.x, 0, 500 + origin.y));
		pointsPlane.push_back(getPointsWithValues(500 + origin.x, 0, -500 + origin.y));
		return true;
	}
	else if(planeEquation.x != 0){
		pointsPlane.push_back(getPointsWithValues(0, -500 + origin.y,500 + origin.z));
		pointsPlane.push_back(getPointsWithValues(0, -500 + origin.y,-500 + origin.z));
		pointsPlane.push_back(getPointsWithValues(0, 500 + origin.y,500 + origin.z));
		pointsPlane.push_back(getPointsWithValues(0, 500 + origin.y,-500 + origin.z));
		return true;
	}
	return false;
}

glm::vec3 Plane::getPointsWithValues(float x, float y, float z){
	if(z == 0) return glm::vec3(x, y, (this->d - x*planeEquation.x - y*planeEquation.y)/planeEquation.z);
	else if(y == 0) return glm::vec3(x, (this->d - x*planeEquation.x - z*planeEquation.z)/planeEquation.y, z);
	else if(x == 0) return glm::vec3((this->d - y*planeEquation.y - z*planeEquation.z)/planeEquation.x, y, z);
}

glm::vec3 Plane::getPointPlaneByIndex(int index){
	return pointsPlane[index];
}

bool Plane::intersect(Plane& p2,Plane& p3){
	glm::mat3 eqSystem(planeEquation,p2.getPlaneEquation(),p3.getPlaneEquation());
	return glm::determinant(eqSystem)>0;
}
glm::vec3 Plane::getPlaneEquation(){
	return planeEquation;
}

float Plane::getPlaneEquationConstant(){
	return d;
}
glm::vec3 Plane::getNormal(){
	return normal;
}
glm::vec3 Plane::getOrigin(){
	return origin;
}

bool Plane::isLeft(glm::vec3 vec){
	return glm::dot(normal,vec-origin)>=0.0f;
}

void Plane::setOrigin(glm::vec3 vec){
	origin = vec;
}

void Plane::setNormal(glm::vec3 vec){
	normal = vec;
}
