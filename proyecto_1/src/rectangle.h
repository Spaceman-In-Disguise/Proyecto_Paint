#pragma once

#ifndef PROYECTO_1_RECTANGLE_H
#define PROYECTO_1_RECTANGLE_H

#include "Shape.h"
#include "canvas.h"
#include <functional>

class Rectangle : public Shape{
public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    Rectangle(Point p0, Point p1, const Color& color, PixelCallback pixelWriter);

    void draw() override;
    void setP1(Point new_p1);
private:
    Point p0;
    Point p1;
    Canvas canvas;
    PixelCallback pixelWriter;
};


#endif //PROYECTO_1_RECTANGLE_H
