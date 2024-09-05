#pragma once

#include "hittable_list.h"
#include "utils.h"

class BVHNode : public Hittable
{
public:
    explicit BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size())
    {
    }

    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end)
    {
        bbox = AABB::empty;
        for (size_t object_index = start; object_index < end; object_index++)
            bbox = AABB(bbox, objects[object_index]->bounding_box());

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

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override
    {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    AABB bounding_box() const override { return bbox; }

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;

    static bool box_compare(
        const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index)
    {
        auto a_axis_interval = a->bounding_box().getAxisInterval(axis_index);
        auto b_axis_interval = b->bounding_box().getAxisInterval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return box_compare(a, b, 2);
    }
};