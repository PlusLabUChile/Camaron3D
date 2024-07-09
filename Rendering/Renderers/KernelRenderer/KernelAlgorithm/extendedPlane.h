#ifndef EXTENDEDPLANE_H
#define EXTENDEDPLANE_H

// extended version of cinolib::plane, with the additional information of three
// points contained in the plane, useful for Shewchuck predicates

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

// represents a plane with equation
// a*x + b*y + c*z = d
// or, using the Hessian Normal Form
// (https://en.wikipedia.org/wiki/Hesse_normal_form) n dot (x,y,z) - d = 0

class ExtendedPlane {

public:
    glm::f64vec3 n;  // plane normal (i.e. a,b,c, coefficients of the plane equation)
    double d; // d coefficient of the plane equation
    glm::f64vec3 p;  // any point on the plane (useful for point_plane_dist)
    std::vector<glm::f64vec3> points; // three points on the plane (the first one is p)

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    explicit ExtendedPlane(const glm::f64vec3 &point = glm::f64vec3(0, 0, 0),
                            const glm::f64vec3 &normal = glm::f64vec3(0, 0, 1));

    explicit ExtendedPlane(const glm::f64vec3 &p0, const glm::f64vec3 &p1, const glm::f64vec3 &p2);

    explicit ExtendedPlane(
        const std::vector<glm::f64vec3> &samples); // best fitting plane

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    double a() const { return n.x; }
    double b() const { return n.y; }
    double c() const { return n.z; }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    void set_plane(const glm::f64vec3 &point, const glm::f64vec3 &normal);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    double operator[](const glm::f64vec3 &p) const;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    bool operator=(const ExtendedPlane & P) const;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    double point_plane_dist_signed(const glm::f64vec3 &p) const;
    double point_plane_dist(const glm::f64vec3 &p) const;
    glm::f64vec3 project_onto(const glm::f64vec3 &p) const;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

};

bool isnan(const glm::f64vec3 &p);
bool isinf(const glm::f64vec3 &p);
bool isdeg(const glm::f64vec3 &p);
bool isnull(const glm::f64vec3 &p);

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

std::ostream &operator<<(std::ostream &in, const ExtendedPlane &plane);

#endif // EXTENDEDPLANE_H
