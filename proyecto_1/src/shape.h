#pragma once

#include "engine2D.h"

class Shape {
public:

    Point position;

    virtual ~Shape() = default;

    virtual void draw() = 0;
};
