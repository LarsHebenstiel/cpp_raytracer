#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "utility.hpp"
#include "hittable.hpp"
#include "aabb.hpp"
#include "material.hpp"

#include <memory>

struct TriangleMesh {
    public:
        const int nVertices, nTriangles;

        // tri (i) has vertices 3i, 3i + 1, 3i + 2
        // this vector holds the indicies of those vertices
        std::vector<int> vertex_indicies;

        std::shared_ptr<material> mat_ptr;

        // the actual vertices
        std::unique_ptr<point3[]> p;

        // the normals for those vertices
        std::unique_ptr<vec3[]> n;

        TriangleMesh(
            int nTriangles, const int *vertex_indices, int nVertices, 
            const point3 *p, const vec3 *n, std::shared_ptr<material> m) 
            : nVertices{ nVertices }, nTriangles{ nTriangles },
              vertex_indicies{ vertex_indices, vertex_indices + 3 * nTriangles}, 
              mat_ptr{ m }
            {
                // copy over data
                this->p.reset(new point3[nVertices]);
                for (int i = 0; i < nVertices; i++) this->p[i] = p[i];

                //if normals are specified, copy them
                if (n) {
                    this->n.reset(new vec3[nVertices]);
                    for (int i = 0; i < nVertices; i++) this->n[i] = n[i];
                }
            }
};

class triangle : public hittable {
    public:
        //triangle parent mesh
        std::shared_ptr<TriangleMesh> mesh;

        //pointer to the index of the first vertex
        // *v is index of first, *v + 1 is index of second
        // actual vertices are held in mesh->p
        const int* v;
        
        //init the shared ptr to mesh, and init v to point towards the first vertex index
        triangle(const std::shared_ptr<TriangleMesh>& mesh, int triNumber)
        : mesh{ mesh } {
            v = &(this->mesh->vertex_indicies[triNumber * 3]);
        }

        virtual bool hit(const ray& r, Float time0, Float time1, hit_record& rec) const override;

        virtual bool bounding_box(Float time0, Float time1, aabb& output_box) const override;
};

bool triangle::bounding_box(Float time0, Float time1, aabb& output_box) const {
    output_box = aabb(mesh->p[v[0]], mesh->p[v[1]]);
    output_box = surrounding_box(output_box, mesh->p[v[2]]);
    return true;
}

/*
 * Moeller Trumbore algorithm for fast ray triangle intersection
 */
bool triangle::hit(const ray& r, Float t_min, Float t_max, hit_record& rec) const {
    point3 &a = mesh->p[v[0]];
    point3 &b = mesh->p[v[1]];
    point3 &c = mesh->p[v[2]];

    vec3 e2 = c - a;
    vec3 e1 = b - a;
    
    vec3 pvec = cross(r.dir, e2); //used for det and baryU
    Float det = (dot(pvec, e1));

    if(det < 1e-5 && det > -1e-5)
        //determinant is zero, happens when ray is parallel to triangle
        return false;

    vec3 tvec = r.orig - a;
    Float inv_det = 1 / det;
    
    Float baryU = dot(pvec, tvec) * inv_det;

    //test to see if baryU is outside of triangle
    if (baryU < 0.0f || baryU > 1.0) {
        return false;
    }

    vec3 qvec = cross(tvec, e1);
    Float baryV = dot(qvec, r.dir) * inv_det;

    //test to see if baryV is outside of triangle
    if (baryV < 0.0 || baryV + baryU > 1.0) {
        return false;
    }

    Float t = dot(qvec, e2) * inv_det;

    if (t < t_min || t > t_max)
        return false;

    Float baryW = 1 - baryU - baryV;

    vec3 n;
    // if we have vertex normal data, interpolate
    if (mesh->n) {
        n = baryU * mesh->n[v[0]] + baryV * mesh->n[v[1]] + baryW * mesh->n[v[2]];
    } else {
        n = unit_vector(cross(e1, e2));
    }

    rec.t = t;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, n);
    rec.mat_ptr = mesh->mat_ptr;

    return true;
}

// if (baryU > 0.03 && baryW > 0.03 & baryV > 0.03 && baryU < 0.97 && baryV < 0.97 && baryW < 0.97)
//         return false;

#endif //TRIANGLE_H