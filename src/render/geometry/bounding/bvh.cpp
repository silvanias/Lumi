#include "bvh.h"
#include <algorithm>

BVHNode::BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size())
{
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end)
{
    bbox = AABB::empty;
    for (size_t objectIndex = start; objectIndex < end; objectIndex++)
        bbox = AABB(bbox, objects[objectIndex]->boundingBox());

    int axis = bbox.longestAxis();

    auto comparator = boxXCompare;
    if (axis == 1)
    {
        comparator = boxYCompare;
    }
    else if (axis == 2)
    {
        comparator = boxZCompare;
    }

    size_t objectSpan = end - start;

    // TODO: optimise later and catch null nodes
    if (objectSpan == 1)
    {
        left = right = objects[start];
    }
    else if (objectSpan == 2)
    {
        left = objects[start];
        right = objects[start + 1];
    }
    else
    {
        // O(nlogn)
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
        auto mid = start + objectSpan / 2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }
}

bool BVHNode::hit(const Ray &r, Interval ray_t, HitRecord &rec) const
{
    if (!bbox.hit(r, ray_t))
        return false;

    bool hitLeft = left->hit(r, ray_t, rec);
    bool hitRight = right->hit(r, Interval(ray_t.min, hitLeft ? rec.t : ray_t.max), rec);

    return hitLeft || hitRight;
}

AABB BVHNode::boundingBox() const { return bbox; }

bool BVHNode::boxCompare(
    const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index)
{
    auto aAxisInterval = a->boundingBox().getAxisInterval(axis_index);
    auto bAxisInterval = b->boundingBox().getAxisInterval(axis_index);
    return aAxisInterval.min < bAxisInterval.min;
}

bool BVHNode::boxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 0);
}

bool BVHNode::boxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 1);
}

bool BVHNode::boxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 2);
}