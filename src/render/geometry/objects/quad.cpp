#include "quad.h"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

Quad::Quad(const glm::vec3 &Q, const glm::vec3 &u, const glm::vec3 &v, std::shared_ptr<Material> mat)
    : Q(Q), u(u), v(v), mat(mat)
{
    glm::vec3 n = cross(u, v); // Normal to the plane containing the quad
    normal = glm::normalize(n);
    D = dot(normal, Q);
    basis_scaling_factor = n / glm::dot(n, n);

    set_bounding_box();
}

void Quad::set_bounding_box()
{
    auto bbox_diagonal1 = AABB(Q, Q + u + v);
    auto bbox_diagonal2 = AABB(Q + u, Q + v);
    bbox = AABB(bbox_diagonal1, bbox_diagonal2);
}

AABB Quad::boundingBox() const { return bbox; }

bool Quad::hit(const Ray &r, Interval ray_t, HitRecord &rec) const
{
    auto denominator = glm::dot(normal, r.direction());
    if (fabs(denominator) < 1e-8)
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

bool Quad::inQuad(double alpha, double beta) const
{
    auto unit_interval = Interval(0, 1);
    return unit_interval.contains(alpha) && unit_interval.contains(beta);
}

std::shared_ptr<HittableList> Box(const glm::vec3 &a, const glm::vec3 &b, std::shared_ptr<Material> mat)
{
    auto sides = std::make_shared<HittableList>();
    auto min = glm::min(a, b);
    auto max = glm::max(a, b);

    glm::vec3 dx(max.x - min.x, 0, 0);
    glm::vec3 dy(0, max.y - min.y, 0);
    glm::vec3 dz(0, 0, max.z - min.z);

    sides->add(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, mat));  // front
    sides->add(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, mat)); // right
    sides->add(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, mat)); // back
    sides->add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, mat));  // left
    sides->add(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, mat)); // top
    sides->add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, mat));  // bottom

    return sides;
}