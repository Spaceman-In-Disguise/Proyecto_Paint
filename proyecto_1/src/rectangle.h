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
    void move(int deltaX, int deltaY) override {
        p0.x += deltaX;
        p0.y += deltaY;
        p1.x += deltaX;
        p1.y += deltaY;
    }
    void setP1(Point new_p1);
private:
    Point p0;
    Point p1;
    Canvas canvas;
    PixelCallback pixelWriter;
};


#endif //PROYECTO_1_RECTANGLE_H
