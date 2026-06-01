#pragma once

#include "engine2D.h"

class Shape {
public:

    Point position = Point(0, 0);
    Point boundingBox[2] = { Point(-1,-1), Point(-1,-1) };
    Color borderColor = Color();
    Color fillColor = Color();
    bool fill = true;
    bool debug = false;
    virtual ~Shape() = default;

    virtual void draw() = 0;
};
