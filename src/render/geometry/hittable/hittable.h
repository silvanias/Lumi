#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "../ray.h"
#include "../interval.h"
#include "../bounding/aabb.h"

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
        // Ray to object space
        Ray offset_ray(r.origin() - offset, r.direction());
        if (!object->hit(offset_ray, ray_t, rec))
            return false;
        // Intersection point to world space
        rec.point += offset;
        return true;
    }

    AABB boundingBox() const override { return bbox; }

private:
    std::shared_ptr<Hittable> object;
    glm::vec3 offset;
    AABB bbox;
};

class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> object, double angle) : object(object)
    {
        sin_theta = std::sin(angle);
        cos_theta = std::cos(angle);
        bbox = object->boundingBox();
        glm::vec3 min(INFINITY, INFINITY, INFINITY);
        glm::vec3 max(-INFINITY, -INFINITY, -INFINITY);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;
                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;
                    glm::vec3 tester(newx, y, newz);
                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }
        bbox = AABB(min, max);
    }

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override
    {
        // Change the ray from world space to object space
        auto origin = r.origin();
        auto direction = r.direction();
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
        Ray rotated_r(origin, direction);
        // Determine whether an intersection exists in object space (and if so, where)
        if (!object->hit(rotated_r, ray_t, rec))
            return false;
        // Change the intersection point from object space to world space
        auto p = rec.point;
        p[0] = cos_theta * rec.point[0] + sin_theta * rec.point[2];
        p[2] = -sin_theta * rec.point[0] + cos_theta * rec.point[2];
        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
        rec.point = p;
        rec.normal = normal;
        return true;
    }

    AABB boundingBox() const override { return bbox; }

private:
    std::shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    AABB bbox;
};