#pragma once

#include "hittable.h"

class Quad : public Hittable
{
public:
    Quad(const glm::vec3 &Q, const glm::vec3 &u, const glm::vec3 &v, std::shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        // Normal to the plane containing the quad
        glm::vec3 n = cross(u, v);
        normal = glm::normalize(n);
        D = dot(normal, Q);
        basis_scaling_factor = n / glm::dot(n, n);

        set_bounding_box();
    }

    void set_bounding_box()
    {
        auto bbox_diagonal1 = AABB(Q, Q + u + v);
        auto bbox_diagonal2 = AABB(Q + u, Q + v);
        bbox = AABB(bbox_diagonal1, bbox_diagonal2);
    }

    AABB boundingBox() const override { return bbox; }

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override
    {
        auto denominator = glm::dot(normal, r.direction());
        if (std::fabs(denominator) < 1e-8)
            return false;
        auto t = (D - dot(normal, r.origin())) / denominator;
        if (!ray_t.contains(t))
            return false;

        auto intersection = r.at(t);
        glm::vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = glm::dot(basis_scaling_factor, glm::cross(planar_hitpt_vector, v));
        auto beta = glm::dot(basis_scaling_factor, glm::cross(u, planar_hitpt_vector));
        if (!inQuad(alpha, beta))
            return false;

        rec.t = t;
        rec.point = intersection;
        rec.mat = mat;
        rec.setFaceNormal(r, normal);

        return true;
    }

    bool inQuad(double alpha, double beta) const
    {
        auto unit_interval = Interval(0, 1);
        if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
            return false;
        return true;
    }

private:
    // A corner
    glm::vec3 Q;
    // Vectors to the other corners
    glm::vec3 u;
    glm::vec3 v;

    glm::vec3 basis_scaling_factor;
    glm::vec3 normal;
    // Offset of the plane from origin
    double D;

    AABB bbox;
    std::shared_ptr<Material> mat;
};