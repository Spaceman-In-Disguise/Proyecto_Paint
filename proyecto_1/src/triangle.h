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
    void setP1(Point new_p1);
    void setP2(Point new_p2);
    [[nodiscard]] Point getP0() const;
private:
    Point p0;
    Point p1;
    Point p2;
    Color color;
    Canvas canvas;
    PixelCallback pixelWriter;
};

#endif //PROYECTO_1_TRIANGLE_H