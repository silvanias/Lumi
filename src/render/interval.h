#pragma once

#include "math.h"

class Interval
{
public:
    double min;
    double max;

    Interval() : min(+INFINITY), max(-INFINITY) {}
    Interval(double min, double max) : min(min), max(max) {}

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

    static const Interval empty;
    static const Interval universe;
};

const Interval Interval::empty = Interval(+INFINITY, -INFINITY);
const Interval Interval::universe = Interval(-INFINITY, +INFINITY);