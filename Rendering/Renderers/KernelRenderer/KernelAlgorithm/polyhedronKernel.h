#ifndef POLYHEDRON_KERNEL_H
#define POLYHEDRON_KERNEL_H

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <QThread>

#include "extendedPlane.h"
#include "sort_points.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Rendering/RModel/rmodel.h"
#include "ModelLoading/MeshProcessor.h"
#include "Utils/VertexUtils.h"
#include "Utils/PolygonUtils.h"


/*
This file (and the others inside KernelAlgorithm folder) was created by 
T. Sorgente, S. Biasotti and M. Spagnuolo as supplemental material for the paper 
"Polyhedron Kernel Computation Using a Geometric Approach".

The folder only contains the code for the computation of the geometric kernel of a polyhedron
*/

class PolyhedronKernel: public QThread {
    Q_OBJECT
    public:
        PolyhedronKernel(){
            model = (Model*)0;
            kmodel = (Model*)0;
            rmodel = (RModel*)0;
        };
        ~PolyhedronKernel(){};
        std::vector<glm::f64vec3> kernel_verts;
        std::vector<std::vector<unsigned int>> kernel_faces;

        void initialize(const std::vector<glm::f64vec3> &verts);

        void compute(
            const std::vector<glm::f64vec3> &verts, const std::vector<std::vector<unsigned int>> &faces,
            const std::vector<glm::f64vec3> &normals, const bool &shuffle = false);


        void call(Model*, RModel*);

        void deleteData();

        enum INTERSECTION_TYPE {
            BELOW = -1,
            INTER = 0,
            ABOVE = 1
        };

        Model* kmodel;
        void run();

    signals:
		void setupProgressBarForNewModel(int,int,int);
		void setLoadedInformation(int);
		void setLoadedComputed(int);
		void setLoadedKernel(int);
		void stageComplete(int);
        void finishCalculateKernel();

    private:
        double TOLL = 1e-8;
        double inf_float = std::numeric_limits<double>::infinity();

        void polyhedron_plane_intersection(
            std::vector<glm::f64vec3> &verts, const std::vector<INTERSECTION_TYPE> &v_sign,
            std::vector<std::vector<uint>> &faces, const ExtendedPlane &p);

        void polygon_plane_intersection(
            std::vector<glm::f64vec3> &verts, std::vector<INTERSECTION_TYPE> &v_sign,
            std::vector<uint> &face, const ExtendedPlane &p);

        glm::f64vec3 line_plane_intersection(
            const glm::f64vec3 &v0, const glm::f64vec3 &v1,
            const ExtendedPlane &p);

        INTERSECTION_TYPE classify(const std::vector<INTERSECTION_TYPE> &sign);

        void add_face(std::vector<uint> &new_f, std::vector<std::vector<uint>> &faces);

        INTERSECTION_TYPE contains(const ExtendedPlane &P, const glm::f64vec3 &p);

        std::vector<glm::f64vec3>::iterator find_v(
            std::vector<glm::f64vec3>::iterator first,
            std::vector<glm::f64vec3>::iterator last,
            const glm::f64vec3 &v);

        std::vector<unsigned int> sort_vector(std::vector<unsigned int>& vect);

        void getInformationModel(
            std::vector<glm::f64vec3>&,
            std::vector<std::vector<unsigned int>>&,
            std::vector<glm::f64vec3>&);
        
        Model* model;
        RModel* rmodel;
        
};

namespace Utils{
    inline double orient3d(glm::f64vec3 pa, glm::f64vec3 pb, glm::f64vec3 pc, glm::f64vec3 pd)
    {
    double adx, bdx, cdx;
    double ady, bdy, cdy;
    double adz, bdz, cdz;

    adx = pa[0] - pd[0];
    bdx = pb[0] - pd[0];
    cdx = pc[0] - pd[0];
    ady = pa[1] - pd[1];
    bdy = pb[1] - pd[1];
    cdy = pc[1] - pd[1];
    adz = pa[2] - pd[2];
    bdz = pb[2] - pd[2];
    cdz = pc[2] - pd[2];

    return adx * (bdy * cdz - bdz * cdy)
        + bdx * (cdy * adz - cdz * ady)
        + cdx * (ady * bdz - adz * bdy);
    }
}

#endif // POLYHEDRON_KERNEL_H