#pragma once

#include "math.h"

// Interval class represents a range of values, useful for bounding ray parameters and axis-aligned intervals.
class Interval
{
public:
    double min;
    double max;

    Interval() : min(+INFINITY), max(-INFINITY) {}
    Interval(double min, double max) : min(min), max(max) {}

    // Constructs an interval encompassing two intervals.
    Interval(const Interval &a, const Interval &b)
        : min(a.min <= b.min ? a.min : b.min), max(a.max >= b.max ? a.max : b.max)
    {
    }

    // Returns the size of the interval.
    double size() const
    {
        return max - min;
    }

    // Checks if the interval contains a value.
    bool contains(double x) const
    {
        return min <= x && x <= max;
    }

    // Checks if a value is strictly within the interval (not on the boundary).
    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    // Clamps a value to the interval's limits.
    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    // Expands the interval by a given delta.
    Interval expand(double delta) const
    {
        auto padding = delta / 2.0f;
        return Interval(min - padding, max + padding);
    }

    // Offsets the entire interval by a displacement.
    Interval operator+(double displacement)
    {
        return Interval(min + displacement, max + displacement);
    }

    static const Interval empty;
    static const Interval universe;
};