#include "extendedPlane.h"

std::ostream &operator<<(std::ostream &in, const ExtendedPlane &plane) {
    in << "[Plane] " << plane.n.x << " * x + " << plane.n.y << " * y + "
        << plane.n.z << " * z = " << plane.d << "\n";
    return in;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ExtendedPlane::ExtendedPlane(const glm::f64vec3 &p0, const glm::f64vec3 &p1,
                             const glm::f64vec3 &p2) {
    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);
    glm::f64vec3 u = p1 - p0;
    glm::f64vec3 v = p2 - p0;
    glm::f64vec3 norm = glm::cross(u,v);

    // if (p0.is_nan() || p0.is_inf() || norm.is_deg()) {
        if(isnan(p0) || isinf(p0) || isdeg(norm)){
        std::cout << "WARNING : failed to set degenerate plane!" << std::endl;
        p = glm::f64vec3(0, 0, 0);
        n = glm::f64vec3(0, 0, 0);
        return;
    }
    p = p0;
    n = norm;
    n = glm::normalize(n);
    d = glm::dot(n, p0);
    assert(fabs(operator[](p0)) < 1e-10);
    assert(fabs(operator[](p1)) < 1e-10);
    assert(fabs(operator[](p2)) < 1e-10);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ExtendedPlane::ExtendedPlane(const glm::f64vec3 &point, const glm::f64vec3 &normal) {
    set_plane(point, normal);
    assert(fabs(operator[](point)) < 1e-10);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// http://www.ilikebigbits.com/blog/2015/3/2/plane-from-points
ExtendedPlane::ExtendedPlane(const std::vector<glm::f64vec3> &samples) {
    // centroid
    glm::f64vec3 c(0, 0, 0);
    for (auto p : samples)
        c += p;
    c /= static_cast<double>(samples.size());
    // 3x3 covariance matrix
    double xx = 0.0;
    double yy = 0.0;
    double xy = 0.0;
    double yz = 0.0;
    double xz = 0.0;
    double zz = 0.0;
    for (auto p : samples) {
        glm::f64vec3 pc = p - c;
        xx += pc.x * pc.x;
        xy += pc.x * pc.y;
        xz += pc.x * pc.z;
        yy += pc.y * pc.y;
        yz += pc.y * pc.z;
        zz += pc.z * pc.z;
    }
    double det_x = yy * zz - yz * yz;
    double det_y = xx * zz - xz * xz;
    double det_z = xx * yy - xy * xy;
    double det_max = std::max(det_x, std::max(det_y, det_z));

    glm::f64vec3 n;
        if (det_max == det_x)
        n = glm::f64vec3(1.0, (xz * yz - xy * zz) / det_x, (xy * yz - xz * yy) / det_x);
        else if (det_max == det_y)
        n = glm::f64vec3((yz * xz - xy * zz) / det_y, 1.0, (xy * xz - yz * xx) / det_y);
        else if (det_max == det_z)
        n = glm::f64vec3((yz * xy - xz * yy) / det_z, (xz * xy - yz * xx) / det_z, 1.0);
        else
        assert(false);

    if (isdeg(n) || glm::length(n) == 0) {
        std::cerr << "WARNING : samples don't span a plane, using method #2"
                << std::endl;
        // http://www.ilikebigbits.com/2017_09_25_plane_from_points_2.html
        xx /= static_cast<double>(samples.size());
        xy /= static_cast<double>(samples.size());
        xz /= static_cast<double>(samples.size());
        yy /= static_cast<double>(samples.size());
        yz /= static_cast<double>(samples.size());
        zz /= static_cast<double>(samples.size());
        glm::f64vec3 weighted_dir(0, 0, 0);
        {
        double det_x = yy * zz - yz * yz;
        glm::f64vec3 axis_dir(det_x, xz * yz - xy * zz, xy * yz - xz * yy);
        double weight = det_x * det_x;
        if (glm::dot(weighted_dir, axis_dir) < 0.0)
            weight = -weight;
        weighted_dir += axis_dir ;
        }
        {
        double det_y = xx * zz - xz * xz;
        glm::f64vec3 axis_dir(xz * yz - xy * zz, det_y, xy * xz - yz * xx);
        double weight = det_y * det_y;
        if (glm::dot(weighted_dir, axis_dir) < 0.0)
            weight = -weight;
        weighted_dir += axis_dir * glm::f64vec3(weight);
        }
        {
        double det_z = xx * yy - xy * xy;
        glm::f64vec3 axis_dir(xy * yz - xz * yy, xy * xz - yz * xx, det_z);
        double weight = det_z * det_z;
        if (glm::dot(weighted_dir, axis_dir) < 0.0)
            weight = -weight;
        weighted_dir += axis_dir * glm::f64vec3(weight);
        }
        n = weighted_dir;
    }
    set_plane(c, n);
    }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void ExtendedPlane::set_plane(const glm::f64vec3 &point, const glm::f64vec3 &normal) {
    if (isnan(point) || isinf(point) || isdeg(normal)){
        std::cout << "WARNING : failed to set degenerate plane!" << std::endl;
        p = glm::f64vec3(0, 0, 0);
        n = glm::f64vec3(0, 0, 0);
        return;
    }
    p = point;
    n = normal;
    n = glm::normalize(n);
    d = glm::dot(n, point);
    assert(fabs(operator[](point)) < 1e-10);

    // find three points on the plane (the first one is p)
    // https://math.stackexchange.com/questions/2563909/find-points-on-a-plane
    double A = n.x, B = n.y, C = n.z;
    double a = p.x, b = p.y, c = p.z;
    glm::f64vec3 s, t;
    if (fabs(A) >= fabs(B) && fabs(A) >= fabs(C)) {
        double u = -B / A;
        double v = -C / A;
        s = glm::f64vec3(a + u, b + 1.0, c);
        t = glm::f64vec3(a + v, b, c + 1.0);
    } else if (fabs(B) >= fabs(A) && fabs(B) >= fabs(C)) {
        double u = -A / B;
        double v = -C / B;
        s = glm::f64vec3(a + 1.0, b + u, c);
        t = glm::f64vec3(a, b + v, c + 1.0);
    } else if (fabs(C) >= fabs(A) && fabs(C) >= fabs(B)) {
        double u = -A / C;
        double v = -B / C;
        s = glm::f64vec3(a + 1.0, b, c + u);
        t = glm::f64vec3(a, b + 1.0, c + v);
    }
    assert(fabs(operator[](s)) < 1e-10);
    assert(fabs(operator[](t)) < 1e-10);

    // order s and t according to the plane normal n
    points.push_back(p);
    glm::f64vec3 N1 = glm::cross(s-p, t-p);
    glm::f64vec3 N2 = glm::cross(t-p, s-p);
    if(glm::dot(N1, n) < 0){
        points.push_back(s);
        points.push_back(t);
    } else if (glm::dot(N2, n) < 0) {
        points.push_back(t);
        points.push_back(s);
    } else
        std::cout << "ERROR : failed to set degenerate plane points!" << std::endl;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double ExtendedPlane::operator[](const glm::f64vec3 &p) const {
  //return (n.dot(p) - d);
  return (glm::dot(n,p) - d);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool ExtendedPlane::operator=(const ExtendedPlane & P) const
{
    //return (fabs(d-P.d) < 1e-10 && n.dist(P.n) < 1e-10);
    return (fabs(d-P.d) < 1e-10 && glm::distance(n, P.n) < 1e-10);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// http://mathworld.wolfram.com/HessianNormalForm.html
// http://mathworld.wolfram.com/Point-PlaneDistance.html (eq. 13)
double ExtendedPlane::point_plane_dist_signed(const glm::f64vec3 &p) const {
    assert(fabs(glm::length(n) - 1.0) < 1e-10);
    glm::f64vec3 u = p - this->p;
    //return u.dot(n);
    return glm::dot(u,n);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

double ExtendedPlane::point_plane_dist(const glm::f64vec3 &p) const {
    return std::fabs(point_plane_dist_signed(p));
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

glm::f64vec3 ExtendedPlane::project_onto(const glm::f64vec3 &p) const {
    glm::f64vec3 res = p - n * glm::f64vec3(point_plane_dist_signed(p));
    auto  err = point_plane_dist(res);
    assert(err < 1e-7);
    return res;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool isnan(const glm::f64vec3 &p) {
	return std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool isinf(const glm::f64vec3 &p) {
	return std::isinf(p.x) || std::isinf(p.y) || std::isinf(p.z);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool isnull(const glm::f64vec3 &p) {
    return (p.x == 0 && p.y == 0 && p.z == 0);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool isdeg(const glm::f64vec3 &p) {
    return isnan(p) || isinf(p) || isnull(p);
}