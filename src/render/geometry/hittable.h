#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "ray.h"
#include "interval.h"
#include "aabb.h"

class Material;

class HitRecord
{
public:
    glm::vec3 point;
    glm::vec3 normal;
    std::shared_ptr<Material> mat;
    float t;
    bool front_face;

    void setFaceNormal(const Ray &r, const glm::vec3 &outward_normal);
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const = 0;

    virtual AABB boundingBox() const = 0;
};