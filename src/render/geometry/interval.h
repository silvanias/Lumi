#pragma once

#include "math.h"

class Interval
{
public:
    double min;
    double max;

    Interval() : min(+INFINITY), max(-INFINITY) {}
    Interval(double min, double max) : min(min), max(max) {}
    // Encompass both
    Interval(const Interval &a, const Interval &b)
        : min(a.min <= b.min ? a.min : b.min), max(a.max >= b.max ? a.max : b.max)
    {
    }

    double size() const
    {
        return max - min;
    }

    bool contains(double x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    static const Interval empty;
    static const Interval universe;
};