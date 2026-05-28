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
    , canvas(pixelWriter) {}

void Rectangle::draw() {
    const Point topLeft = p0;
    const auto topRight = Point(p1.x, p0.y);
    const Point bottomRight = p1;
    const auto bottomLeft = Point(p0.x, p1.y);

    //Horizontal First (From the Left)
    Line* l1 = canvas.addLine(topLeft, color);
    Line* l2 = canvas.addLine(bottomLeft, color);

    canvas.resizeLine(topRight, l1);
    canvas.resizeLine(bottomRight, l2);

    //Then Verticals (From the Top)
    Line* l3 = canvas.addLine(topLeft, color);
    Line* l4 = canvas.addLine(topRight, color);

    canvas.resizeLine(bottomLeft, l3);
    canvas.resizeLine(bottomRight, l4);

    canvas.draw();
}
