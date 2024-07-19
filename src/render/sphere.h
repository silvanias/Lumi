#pragma once

#include <glm/gtx/norm.hpp>
#include "hittable.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &center, float radius) : center(center), radius(radius) {}

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override
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
        rec.p = r.at(rec.t);
        glm::vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.normal = (rec.p - center) / radius;

        return true;
    }

private:
    glm::vec3 center;
    float radius;
};
