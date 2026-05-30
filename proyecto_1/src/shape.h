#pragma once

#include "engine2D.h"

class Shape {
public:

    Point position;
    bool fill = true;
    bool debug = false;
    virtual ~Shape() = default;

    virtual void draw() = 0;
};
