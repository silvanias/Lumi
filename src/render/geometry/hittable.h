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

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> object, const glm::vec3 &offset) : object(object), offset(offset)
    {
        bbox = object->boundingBox() + offset;
    }

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override
    {
        Ray offset_ray(r.origin() - offset, r.direction());
        if (!object->hit(offset_ray, ray_t, rec))
            return false;
        rec.point += offset;
        return true;
    }

    AABB boundingBox() const override { return bbox; }

private:
    std::shared_ptr<Hittable> object;
    glm::vec3 offset;
    AABB bbox;
};