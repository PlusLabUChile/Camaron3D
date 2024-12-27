#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
#include <vector>
class Plane
{
	public:
		Plane(glm::vec3 planeEq, float nd);
		Plane(glm::vec3 norigin, glm::vec3 v1,glm::vec3 v2);
		Plane(glm::vec3 norigin, glm::vec3 nnormal);
		Plane(float a,float b, float c, float d);
		/* @note Setting this destructor to virtual will trigger an illegal
		   instruction, specifically when attempting to repopulate a recently 
		   cleared vector of plane objects. Still unsure why this happens...*/
		~Plane();
		///
		glm::vec3 getPointOfIntersection(Plane&,Plane&);
		glm::vec3 getPlaneEquation();
		glm::vec3 getNormal();
		glm::vec3 getOrigin();
		float getPlaneEquationConstant();
		bool intersect(Plane&,Plane&);
		void fixNormal(glm::vec3);
		bool isOk();
		bool isLeft(glm::vec3);

		void setOrigin(glm::vec3);
		void setNormal(glm::vec3);
		bool getPointsPlane();
		void setLengthPlane(float);
		glm::vec3 getPointPlaneByIndex(int);
		glm::vec3 getPointsWithValues(float, float, float);

	private:
		std::vector<glm::vec3> pointsPlane;
		glm::vec3 normal;
		glm::vec3 origin;
		//a*x + b*y + c*z = d
		glm::vec3 planeEquation; // a,b,c
		float d;// d
		float length;
		bool ok;
};

#endif // PLANE_H
