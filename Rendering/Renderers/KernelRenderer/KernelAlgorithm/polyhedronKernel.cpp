#include "polyhedronKernel.h"
#include <random>
#include <algorithm>
#include <iostream>


void PolyhedronKernel::call(Model* model, RModel* rmodel){
    this->model = model;
    this->rmodel = rmodel;
    std::cout << model->getFilename() << std::endl;
    this->start();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void PolyhedronKernel::deleteData(){
    this->model = (Model*)0;
    this->rmodel = (RModel*)0;
    delete kmodel;
    this->kmodel = 0;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


void PolyhedronKernel::getInformationModel(std::vector<glm::f64vec3> &verts,
    std::vector<std::vector<unsigned int>> &faces, std::vector<glm::f64vec3> &normals){
    ElementsRelations* relations = this->model->getElementsRelations();

    verts.reserve(this->model->getVerticesCount());
    faces.reserve(this->model->getPolygonsCount());

    for(int i = 0; i < model->getVerticesCount(); i++){
        verts.emplace_back(model->getVertices().at(i).getCoords());
        if(i%100==0) emit setLoadedInformation(i);
    }

    for(int i = 0; i < model->getPolygonsCount(); i++){
        if(PolygonUtils::isPolygonAtSurface(this->model, &this->model->getPolygons().at(i))){
            std::vector<unsigned int> vFaces;
            vFaces.reserve(relations->getVertexPolygonsById(i).size());
            for(unsigned int vid: relations->getVertexPolygonsById(i)){
                vFaces.emplace_back(relations->getPositionInContainerById(vid));
            }
            //faces.emplace_back(relations->getVertexPolygonsById(i));
            faces.emplace_back(vFaces);
            normals.emplace_back(model->getPolygons().at(i).getNormal());
        }
        if(i%100==0) emit setLoadedInformation(verts.size() + i);
    }
    emit setLoadedInformation(verts.size() + faces.size());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::run(){
    // First step: Get model info (vertices, faces and normals)
    std::vector<glm::f64vec3> verts;
	std::vector<std::vector<unsigned int>> faces;
	std::vector<glm::f64vec3> normals;
    getInformationModel(verts, faces, normals);

    // Second step: compute kernel
    initialize(verts);
    compute(verts, faces, normals);

    // Third step: create kernel model 
    kmodel = new Model("kernel");
    ElementsRelations* relations = kmodel->getElementsRelations();
	std::vector<vis::Vertex>& vec = kmodel->getVertices();
	vec.reserve(kernel_verts.size());
	for(unsigned int i = 0; i < kernel_verts.size(); i++){
		vec.emplace_back(i, kernel_verts[i].x, kernel_verts[i].y, kernel_verts[i].z);
        if(i%100 == 0) emit setLoadedKernel(i);
	}

	std::vector<vis::Polygon>& vecPol = kmodel->getPolygons();
	vecPol.reserve(kernel_faces.size());
	relations->getVertexPolygons().reserve(kernel_faces.size());
	for(unsigned int i = 0; i < kernel_faces.size(); i++){
		vecPol.emplace_back(i);
		relations->getVertexPolygons().emplace_back();
		for(unsigned int idVertex : kernel_faces[i]){
			relations->addVertexInPolygon(i, idVertex);
		}
        if(i%100 == 0) emit setLoadedKernel(kernel_verts.size() + i);
	}
	MeshProcessor::completeVertexPolygonRelations(kmodel);
	MeshProcessor::calculateNormalsPolygons(kmodel);
	MeshProcessor::calculateNormalsVertices(kmodel);

    emit setLoadedKernel(kernel_verts.size() + kernel_faces.size());
    emit finishCalculateKernel();
    kernel_verts.clear();
    kernel_faces.clear();
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::initialize(const std::vector<glm::f64vec3> &verts){
    // Initialize the kernel with the polyhedron AABB
    if (verts.empty()) return;

    glm::f64vec3 min(inf_float,inf_float,inf_float);
    glm::f64vec3 max(-inf_float,-inf_float,-inf_float);

    for(const glm::f64vec3 &p : verts){
        // tener ojito aqui
        min = glm::f64vec3(glm::min(min, p));
        max = glm::f64vec3(glm::max(max, p));
    }
    kernel_verts = {
        min,
        glm::f64vec3(min.x, max.y, min.z),
        glm::f64vec3(max.x, max.y, min.z),
        glm::f64vec3(max.x, min.y, min.z),
        glm::f64vec3(min.x, min.y, max.z),
        glm::f64vec3(min.x, max.y, max.z),
        max,
        glm::f64vec3(max.x, min.y, max.z),
    };

    kernel_faces = {{0, 1, 2, 3}, {2, 1, 5, 6}, {3, 2, 6, 7},
                    {0, 3, 7, 4}, {1, 0, 4, 5}, {5, 4, 7, 6}};

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::compute(
            const std::vector<glm::f64vec3> &verts, const std::vector<std::vector<unsigned int>> &faces,
            const std::vector<glm::f64vec3> &normals, const bool &shuffle) {
    // Initial function to compute Geometric Polyhedron Kernel
    if (kernel_verts.empty() || kernel_faces.empty()){
        std::cout << "ERROR: initialize kernel before computing." << std::endl;
        return;
    }

    // Shuffle faces of Polyhedron (optional)
    std::vector<unsigned int> faces_ids(faces.size());
    std::iota(faces_ids.begin(), faces_ids.end(), 0);
    
    if(shuffle){
        std::random_device rd;
        std::default_random_engine g(rd());
        std::shuffle(faces_ids.begin(), faces_ids.end(), g);
    }

    // Iterate all faces of Polyhedron
    for (unsigned int i = 0; i < faces_ids.size(); i++){
        // Get vertices of that face and check if it's valid
        std::vector<unsigned int> f = faces.at(faces_ids.at(i));
        std::vector<glm::f64vec3> v(f.size());
        for(unsigned int vid = 0; vid < f.size(); vid++)
            v.at(vid) = verts.at(f.at(vid));
        if(f.size() == 0 || isnan(v.front()) || isinf(v.front()) || 
                isdeg(normals.at(faces_ids.at(i)))){
            std::cout << "WARNING: skipping degenerate face." << std::endl;
            continue;
        }
        // Create the plane in Hessian form
        ExtendedPlane plane(v.front(), -normals.at(faces_ids.at(i))); 
        // Create Sign vector and set the value for each vertex
        std::vector<INTERSECTION_TYPE> vertex_sign(kernel_verts.size());
        for(unsigned int vid = 0; vid < kernel_verts.size(); vid++){
            if(find_v(v.begin(), v.end(), kernel_verts.at(vid)) != v.cend()) 
                vertex_sign.at(vid) = INTER;
            else 
                vertex_sign.at(vid) = contains(plane, kernel_verts.at(vid));
        }
        // Call 2nd Algorithm
        polyhedron_plane_intersection(kernel_verts, vertex_sign, kernel_faces, plane);
        if(kernel_verts.size() < 3 || kernel_faces.size() < 3){
            kernel_faces.clear();
            kernel_verts.clear();
            std::cout << "No Kernel\n";
            return;
        }
        if(i%250 == 0) emit setLoadedComputed(i);
    }
    emit setLoadedComputed(faces_ids.size());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::polyhedron_plane_intersection(
            std::vector<glm::f64vec3> &verts, const std::vector<INTERSECTION_TYPE> &v_sign,
            std::vector<std::vector<unsigned int>> &faces, const ExtendedPlane &p){
    // Set vectors that they will replace verts and faces
    std::vector<glm::f64vec3> above_v;
    std::vector<INTERSECTION_TYPE> above_s;
    std::vector<std::vector<unsigned int>> above_f;

    for(unsigned int fid = 0; fid < faces.size(); fid++){
        std::vector<unsigned int> f = faces.at(fid);
        std::vector<INTERSECTION_TYPE> fs(f.size());

        for (unsigned int vid = 0; vid < f.size(); vid++)
            fs.at(vid) = v_sign.at(f.at(vid));
        
        switch (classify(fs)){
            case BELOW: {
                break;
            }
            case ABOVE: {
                for(unsigned int &vid : f){
                    auto it = find_v(above_v.begin(), above_v.end(), verts.at(vid));
                    if(it == above_v.end()){
                        above_v.push_back(verts.at(vid));
                        above_s.push_back(v_sign.at(vid));
                        vid = above_v.size() - 1; 
                    }
                    else 
                        vid = it - above_v.cbegin();
                }
                above_f.push_back(f);
                break;
            }
            case INTER: {
                std::vector<glm::f64vec3> fv(f.size());
                for(unsigned int vid = 0; vid < f.size(); vid++)
                    fv.at(vid) = verts.at(f.at(vid));
                
                polygon_plane_intersection(fv, fs, f, p);

                for(unsigned int i = 0; i < f.size(); i++){
                    auto it = find_v(above_v.begin(), above_v.end(), fv.at(i));
                    if(it == above_v.end()){
                        above_v.push_back(fv.at(i));
                        above_s.push_back(fs.at(i));
                        f.at(i) = above_v.size() - 1;
                    }
                    else
                        f.at(i) = it - above_v.cbegin();
                }
                add_face(f, above_f);
                break;
            }
            default: break;
        }
    }

    verts = above_v;
    faces = above_f;

    std::vector<glm::f64vec3> cap_v;
    std::vector<unsigned int> cap_vids;

    for (unsigned int vid = 0; vid < above_v.size(); vid++)
        if (above_s.at(vid) == INTER) {
            cap_vids.push_back(vid);
            cap_v.push_back(above_v.at(vid));
        }
    if (cap_v.size() < 3)
        return;
    std::vector<unsigned int> cap_f(cap_v.size());
    std::vector<unsigned int> tmp_f = Utils::sort_points(cap_v, p);
    for (unsigned int i = 0; i < tmp_f.size(); i++)
        cap_f.at(i) = cap_vids.at(tmp_f.at(i));
    add_face(cap_f, faces);

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::polygon_plane_intersection(
        std::vector<glm::f64vec3> &verts, std::vector<INTERSECTION_TYPE> &v_sign,
        std::vector<unsigned int> &face, const ExtendedPlane &p){
    std::vector<glm::f64vec3> above_v;
    std::vector<INTERSECTION_TYPE> above_s;
    std::vector<unsigned int> above_f;
    unsigned int last = *std::max_element(face.cbegin(), face.cend());

    for (unsigned int eid = 0; eid < face.size(); eid++) {
        unsigned int vid1 = face.at((eid + 1) % face.size());
        glm::f64vec3 v1 = verts.at((eid + 1) % verts.size());
        INTERSECTION_TYPE vs0 = v_sign.at(eid);
        INTERSECTION_TYPE vs1 = v_sign.at((eid + 1) % v_sign.size());
        switch (classify({vs0, vs1})) {
        case BELOW: { // edge weakly below the plane
        if (vs1 == INTER) {
            above_v.push_back(v1);
            above_s.push_back(vs1);
            above_f.push_back(vid1);
        }
        break;
        }
        case ABOVE: { // edge weakly above the plane
        above_v.push_back(v1);
        above_s.push_back(vs1);
        above_f.push_back(vid1);
        break;
        }
        case INTER: { // edge properly intersects the plane
        glm::f64vec3 v = line_plane_intersection(verts.at(eid), v1, p);
        last++;
        above_v.push_back(v);
        above_s.push_back(INTER);
        above_f.push_back(last);
        if (vs0 == BELOW) {
            above_v.push_back(v1);
            above_s.push_back(vs1);
            above_f.push_back(vid1);
        }
        break;
        }
        default:
        break;
        }
    }
    verts = above_v;
    v_sign = above_s;
    face = above_f;
    assert(verts.size() == face.size());
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

glm::f64vec3 PolyhedronKernel::line_plane_intersection(
        const glm::f64vec3 &v0, const glm::f64vec3 &v1, const ExtendedPlane &plane){
  glm::f64vec3 p = glm::distance(v0, plane.p) > TOLL ? plane.p : plane.points.at(1);
  double N = glm::dot(plane.n, v0 - p);
  double D = glm::dot(plane.n, v1 - v0);
  assert(fabs(D) > TOLL);
  glm::f64vec3 result = v0 - (glm::f64vec3(N) / glm::f64vec3(D)) * (v1 - v0);
  return result;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PolyhedronKernel::INTERSECTION_TYPE 
PolyhedronKernel::classify(const std::vector<INTERSECTION_TYPE> &sign){
    int size = sign.size();
    assert(size > 1);
    switch (size)
    {
    case 2 : { // Edge
        INTERSECTION_TYPE s0 = sign.front();
        INTERSECTION_TYPE s1 = sign.back();
        if (s0 <= 0 && s1 <= 0)
            return BELOW;
        else if (s0 >= 0 && s1 >= 0)
            return ABOVE;
        else
            return INTER;
        break;
    }
    default: { // face
        int N = std::count(sign.cbegin(), sign.cend(), BELOW);
        if (N == size)
            return BELOW;
        else if (N == 0)
            return ABOVE;
        else
            return INTER;
        break;
    }
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void PolyhedronKernel::add_face(std::vector<unsigned int> &new_face, std::vector<std::vector<unsigned int>> &faces){
    if(new_face.size() < 3) return;
    std::vector<unsigned int> temp = sort_vector(new_face);
    for(std::vector<unsigned int> &face : faces){
        if(temp == sort_vector(face)) return; // is alredy in faces
    }
    faces.push_back(new_face);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


PolyhedronKernel::INTERSECTION_TYPE PolyhedronKernel::contains(const ExtendedPlane& P, const glm::f64vec3 &p){
    glm::f64 d = Utils::orient3d(P.points.at(0), P.points.at(1), P.points.at(2), p);
    if (fabs(d) < TOLL)
        return INTER;
    else if (d > 0)
        return ABOVE;
    else
        return BELOW;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


std::vector<glm::f64vec3>::iterator PolyhedronKernel::find_v(std::vector<glm::f64vec3>::iterator first,
        std::vector<glm::f64vec3>::iterator last, const glm::f64vec3 &v) {
    for (; first != last; ++first)
        if (fabs((*first).x - v.x) < TOLL)
            if (fabs((*first).y - v.y) < TOLL)
            if (fabs((*first).z - v.z) < TOLL)
                break;
    return first;
  }

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

std::vector<unsigned int> PolyhedronKernel::sort_vector(std::vector<unsigned int>& vect){
    std::vector<unsigned int> temp = vect;
    std::sort(temp.begin(), temp.end());
    std::reverse(temp.begin(), temp.end());
    return temp;
}