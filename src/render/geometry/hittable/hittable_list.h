#pragma once

#include <memory>
#include <vector>
#include "hittable.h"
#include "geometry/bounding/aabb.h"
#include "geometry/interval.h"
#include "geometry/ray.h"

class HittableList : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() = default;
    explicit HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    // Adds an object to the list and updates the bounding box.
    void add(std::shared_ptr<Hittable> object);

    HittableList &operator+=(const HittableList &other);

    // Checks if a ray hits any object in the list, updating the hit record.
    bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override;

    // Returns the axis-aligned bounding box enclosing all objects in the list.
    AABB boundingBox() const override;

private:
    AABB bbox;
};