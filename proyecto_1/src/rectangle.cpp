//
// Created by ManguitoRTX on 5/27/2026.
//

#include "rectangle.h"

#include "line.h"

#include <cstdlib>
#include <utility>

Rectangle::Rectangle(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0)
    , p1(p1)
    , color(color)
    , canvas(pixelWriter)
    , pixelWriter(std::move(pixelWriter)) {}

void Rectangle::draw() {
    canvas = Canvas(pixelWriter);

    const Point topLeft = p0;
    const Point topRight(p1.x, p0.y);
    const Point bottomRight = p1;
    const Point bottomLeft(p0.x, p1.y);

    Line* topEdge = canvas.addLine(topLeft, color);
    Line* leftEdge = canvas.addLine(bottomLeft, color);
    canvas.resizeLine(topRight, topEdge);
    canvas.resizeLine(bottomRight, leftEdge);

    Line* bottomEdge = canvas.addLine(topLeft, color);
    Line* rightEdge = canvas.addLine(topRight, color);
    canvas.resizeLine(bottomLeft, bottomEdge);
    canvas.resizeLine(bottomRight, rightEdge);

    canvas.draw();
}
