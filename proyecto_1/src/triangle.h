#pragma once

#ifndef PROYECTO_1_TRIANGLE_H
#define PROYECTO_1_TRIANGLE_H

#include "Shape.h"
#include "canvas.h"
#include <functional>

class Triangle : public Shape {
public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    Triangle(Point p0, Point p1, Point p2, const Color& color, PixelCallback pixelWriter);

    void draw() override;
    void move(int deltaX, int deltaY) override {
        p0.x += deltaX;
        p0.y += deltaY;
        p1.x += deltaX;
        p1.y += deltaY;
        p2.x += deltaX;
        p2.y += deltaY;
    }
    void setP0(Point new_p0);
    void setP1(Point new_p1);
    void setP2(Point new_p2);
    //Getter for p0, p1 and p2
    Point getP0() const { return p0; }
    Point getP1() const { return p1; }
    Point getP2() const { return p2; }
private:
    Point p0;
    Point p1;
    Point p2;
    Canvas canvas;
    PixelCallback pixelWriter;
};

#endif //PROYECTO_1_TRIANGLE_H