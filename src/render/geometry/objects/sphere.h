#pragma once

#include <glm/gtx/norm.hpp>
#include "geometry/hittable/hittable.h"
#include "geometry/ray.h"
#include "../interval.h"
#include "geometry/bounding/aabb.h"
#include "material/material.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> material);

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;
    AABB boundingBox() const override;

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> mat;
    AABB bbox;
};