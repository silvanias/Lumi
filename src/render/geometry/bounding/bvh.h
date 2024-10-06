#pragma once

#include "../hittable/hittable.h"
#include "../hittable/hittable_list.h"
#include "utils.h"
#include "geometry/bounding/aabb.h"

// BVHNode class represents a node in a Bounding Volume Hierarchy, used for efficient ray-object intersection.
class BVHNode : public Hittable
{
public:
    // Constructs a BVHNode from a list of hittable objects.
    explicit BVHNode(HittableList list);

    // Constructs a BVHNode from a range of hittable objects.
    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end);

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;

    // Returns the axis-aligned bounding box of the BVHNode.
    AABB boundingBox() const override;

private:
    std::shared_ptr<Hittable> left;  // Left child
    std::shared_ptr<Hittable> right; // Right child
    AABB bbox;                       // Bounding box for the node

    // Compares two hittable objects along a given axis (so we can sort them into left/right nodes).
    static bool boxCompare(
        const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index);

    // Comparison functions for sorting along X, Y, Z axes.
    static bool boxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool boxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
    static bool boxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
};