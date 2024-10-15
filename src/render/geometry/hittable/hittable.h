#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include "../ray.h"
#include "../interval.h"
#include "../bounding/aabb.h"

// Forward declaration of Material class.
class Material;

// Record of a ray-object intersection.
class HitRecord
{
public:
    glm::vec3 point;               // Intersection point
    glm::vec3 normal;              // Normal at the intersection
    std::shared_ptr<Material> mat; // Material of the intersected object
    float t;                       // Ray parameter at intersection
    bool frontFace;                // Whether the intersection is a front face

    // Sets the normal depending on the ray direction to ensure it always points against the ray.
    void setFaceNormal(const Ray &r, const glm::vec3 &outwardNormal);
};

// Abstract class representing a hittable object in the scene.
class Hittable
{
public:
    virtual ~Hittable() = default;

    // Determines if the ray hits the object and fills in hit record.
    virtual bool hit(const Ray &r, Interval rayT, HitRecord &rec) const = 0;

    // Returns the axis-aligned bounding box of the object.
    virtual AABB boundingBox() const = 0;

    virtual double pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
    {
        return 0.0;
    }

    virtual glm::vec3 random(const glm::vec3 &origin) const
    {
        return glm::vec3(1, 0, 0);
    }
};

// Class for applying a translation to a hittable object.
class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> object, const glm::vec3 &offset) : object(object), offset(offset)
    {
        bbox = object->boundingBox() + offset;
    }

    bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override
    {
        // Ray to object space
        Ray offsetRay(r.origin() - offset, r.direction());
        if (!object->hit(offsetRay, rayT, rec))
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

// Class for applying a Y-axis rotation to a hittable object.
class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> object, double angle) : object(object)
    {
        sinTheta = std::sin(angle);
        cosTheta = std::cos(angle);
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
                    auto newx = cosTheta * x + sinTheta * z;
                    auto newz = -sinTheta * x + cosTheta * z;
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

    bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override
    {
        // Change the ray from world space to object space
        auto origin = r.origin();
        auto direction = r.direction();
        origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
        origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];
        direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
        direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];
        Ray rotated_r(origin, direction);
        // Determine whether an intersection exists in object space (and if so, where)
        if (!object->hit(rotated_r, rayT, rec))
            return false;
        // Change the intersection point from object space to world space
        auto p = rec.point;
        p[0] = cosTheta * rec.point[0] + sinTheta * rec.point[2];
        p[2] = -sinTheta * rec.point[0] + cosTheta * rec.point[2];
        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
        normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];
        rec.point = p;
        rec.normal = normal;
        return true;
    }

    AABB boundingBox() const override { return bbox; }

private:
    std::shared_ptr<Hittable> object;
    double sinTheta;
    double cosTheta;
    AABB bbox;
};