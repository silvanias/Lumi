#include "bvh.h"
#include <algorithm>

BVHNode::BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size())
{
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end)
{
    bbox = AABB::empty;
    for (size_t object_index = start; object_index < end; object_index++)
        bbox = AABB(bbox, objects[object_index]->boundingBox());

    int axis = bbox.longestAxis();

    auto comparator = box_x_compare;
    if (axis == 1)
    {
        comparator = box_y_compare;
    }
    else if (axis == 2)
    {
        comparator = box_z_compare;
    }

    size_t object_span = end - start;

    // TODO: optimise later and catch null nodes
    if (object_span == 1)
    {
        left = right = objects[start];
    }
    else if (object_span == 2)
    {
        left = objects[start];
        right = objects[start + 1];
    }
    else
    {
        // O(nlogn)
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }
}

bool BVHNode::hit(const Ray &r, Interval ray_t, HitRecord &rec) const
{
    if (!bbox.hit(r, ray_t))
        return false;

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

    return hit_left || hit_right;
}

AABB BVHNode::boundingBox() const { return bbox; }

bool BVHNode::box_compare(
    const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index)
{
    auto a_axis_interval = a->boundingBox().getAxisInterval(axis_index);
    auto b_axis_interval = b->boundingBox().getAxisInterval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 2);
}