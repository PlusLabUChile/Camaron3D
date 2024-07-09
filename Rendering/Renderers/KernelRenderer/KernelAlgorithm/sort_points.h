#ifndef SORT_POINTS_H
#define SORT_POINTS_H

#include "extendedPlane.h"
#include <glm/glm.hpp>
#include <algorithm>

namespace Utils{
inline bool compare(const std::pair<uint, glm::f64vec2> &p1,
             const std::pair<uint, glm::f64vec2> &p2) {
  glm::f64vec2 c(0, 0);
  double a1 = atan2(p1.second.y - c.y, p1.second.x - c.x);
  if (a1 <= 0)
    a1 += 2 * M_PI;
  double a2 = atan2(p2.second.y - c.y, p2.second.x - c.x);
  if (a2 <= 0)
    a2 += 2 * M_PI;
  if (a1 != a2)
    return (a1 < a2);
  else
    return (glm::distance(c, p1.second) < glm::distance(c, p2.second));
}

inline glm::f64mat3 mat_set_rot_3d(const double ang_rad, const glm::f64vec3 axis)
{
    glm::f64mat3 m;
    double u     = axis[0];
    double v     = axis[1];
    double w     = axis[2];
    double rcos  = cos(ang_rad);
    double rsin  = sin(ang_rad);
    m[0][0] =      rcos + u*u*(1-rcos);
    m[1][0] =  w * rsin + v*u*(1-rcos);
    m[2][0] = -v * rsin + w*u*(1-rcos);
    m[0][1] = -w * rsin + u*v*(1-rcos);
    m[1][1] =      rcos + v*v*(1-rcos);
    m[2][1] =  u * rsin + w*v*(1-rcos);
    m[0][2] =  v * rsin + u*w*(1-rcos);
    m[1][2] = -u * rsin + v*w*(1-rcos);
    m[2][2] =      rcos + w*w*(1-rcos);
    return m;
} 


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// equivalent of cinolib's 'polygon_flatten' contained in
// geometry/polygon_utils.h using the ExtendedPlane class instead of Plane
inline bool polygon_flatten_Ext(const std::vector<glm::f64vec3> &poly3d,
                         std::vector<glm::f64vec2> &poly2d) {
  poly2d.clear();
  poly2d.reserve(poly3d.size());

  ExtendedPlane best_fit(poly3d);
  // if (best_fit.n.is_deg() || best_fit.n.norm() == 0)
  if(isdeg(best_fit.n) || glm::length(best_fit.n) == 0)
    return false;

  glm::f64vec3 Z = glm::f64vec3(0, 0, 1);
  glm::f64vec3 axis = glm::cross(best_fit.n, Z);
  double angle = glm::acos(glm::dot(best_fit.n, Z) / (glm::length(best_fit.n) * glm::length(Z)));
  glm::f64mat3 R = Utils::mat_set_rot_3d(angle, axis);

  for (auto p : poly3d) {
    glm::f64vec3 tmp = best_fit.project_onto(p);
    tmp = R * tmp;
    poly2d.push_back(glm::f64vec2(tmp[0], tmp[1])); // will drop z
  }
  return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// https://www.baeldung.com/cs/sort-points-clockwise
inline std::vector<uint> sort_points(const std::vector<glm::f64vec3> &verts,
                              const ExtendedPlane &plane) {
  std::vector<glm::f64vec2> points;
  std::vector<uint> vids;
  if (!polygon_flatten_Ext(verts, points)) {
    std::cout << "WARNING: polygon_flatten(verts, points) failed!" << std::endl;
    return vids;
  }

  glm::vec2 c(0, 0); // polygon centroid
  for (const glm::f64vec2 &v : points)
    c += v;
  c /= static_cast<double>(points.size());
  for (glm::f64vec2 &p : points)
    p -= c;

  std::vector<std::pair<uint, glm::f64vec2>> points_map;
  for (uint vid = 0; vid < points.size(); vid++)
    points_map.push_back(std::pair<uint, glm::f64vec2>(vid, points.at(vid)));
  std::sort(points_map.begin(), points_map.end(), Utils::compare);
  for (auto &p : points_map)
    vids.push_back(p.first);

  // plane points towards the interior of the element
  // I want the points to be ordered counterclockwise from outside the element
  // the normal induced by the points has to be opposite wrt the plane normal
  glm::f64vec3 u = verts.at(vids.at(1)) - verts.at(vids.at(0));
  glm::f64vec3 v = verts.at(vids.at(2)) - verts.at(vids.at(0));
  glm::f64vec3 w = glm::cross(u,v);
  if (glm::dot(w, plane.n) > 0)
    std::reverse(vids.begin(), vids.end());
  return vids;
}
}
#endif // SORT_POINTS_H