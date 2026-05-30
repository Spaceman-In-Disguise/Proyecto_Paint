#pragma once

#ifndef PROYECTO_1_ELLIPSE_H
#define PROYECTO_1_ELLIPSE_H

#include "Shape.h"
#include <functional>

class Ellipse : public Shape {
public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    Ellipse(Point p0, Point p1, const Color& color, PixelCallback pixelWriter);

    void draw() override;
    void setP1(Point new_p1);
private:
    Point p0;
    Point p1;
    Color color;
    PixelCallback pixelWriter;
};

#endif //PROYECTO_1_ELLIPSE_H