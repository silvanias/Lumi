#include "sphere.h"

Sphere::Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> material)
    : center(center), radius(radius), mat(material)
{
    auto radius_vec = glm::vec3(radius, radius, radius);
    bbox = AABB(center - radius_vec, center + radius_vec);
}

bool Sphere::hit(const Ray &r, Interval ray_t, HitRecord &rec) const
{
    glm::vec3 origin_to_center = center - r.origin();
    float a = glm::length2(r.direction());
    float h = dot(r.direction(), origin_to_center);
    float c = glm::length2(origin_to_center) - radius * radius;
    float discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = sqrt(discriminant);
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    glm::vec3 outward_normal = (rec.point - center) / radius;
    rec.setFaceNormal(r, outward_normal);
    rec.normal = (rec.point - center) / radius;
    rec.mat = mat;

    return true;
}

AABB Sphere::boundingBox() const { return bbox; }