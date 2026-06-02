#pragma once

#ifndef PROYECTO_1_ELLIPSE_H
#define PROYECTO_1_ELLIPSE_H

#include "Shape.h"
#include <functional>

class Ellipse : public CloneableShape<Ellipse> {
public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    Ellipse(Point p0, Point p1, const Color& color, PixelCallback pixelWriter);

    void draw() override;
    void move(int deltaX, int deltaY) override {
        p0.x += deltaX;
        p0.y += deltaY;
        p1.x += deltaX;
        p1.y += deltaY;
    }
    void setP0(Point new_p0);
    void setP1(Point new_p1);

    //Getter for p0 and p1
    Point getP0() const { return p0; }
    Point getP1() const { return p1; }
private:
    Point p0;
    Point p1;
    PixelCallback pixelWriter;
};

#endif //PROYECTO_1_ELLIPSE_H