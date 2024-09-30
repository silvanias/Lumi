#pragma once

#include "../hittable/hittable.h"
#include "../hittable/hittable_list.h"
#include "utils.h"
#include "geometry/bounding/aabb.h"

class BVHNode : public Hittable
{
public:
    explicit BVHNode(HittableList list);
    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end);

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;
    AABB boundingBox() const override;

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;

    static bool box_compare(
        const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index);

    static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
};