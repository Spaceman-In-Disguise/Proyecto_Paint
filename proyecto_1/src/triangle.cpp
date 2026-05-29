//
// Created by ManguitoRTX on 5/28/2026.
//

#include "triangle.h"

#include "line.h"

#include <utility>

Triangle::Triangle(Point p0, Point p1, Point p2, const Color& color, PixelCallback pixelWriter)
    : p0(p0)
    , p1(p1)
    , p2(p2)
    , color(color)
    , canvas(pixelWriter)
    , pixelWriter(std::move(pixelWriter)) {}

void Triangle::draw() {
    canvas = Canvas(pixelWriter);

    Line* edgeA = canvas.addLine(p0, color);
    Line* edgeB = canvas.addLine(p1, color);
    Line* edgeC = canvas.addLine(p2, color);

    canvas.resizeLine(p1, edgeA);
    canvas.resizeLine(p2, edgeB);
    canvas.resizeLine(p0, edgeC);

    canvas.draw();
}

void Triangle::setP1(Point new_p1) {
    this->p1 = new_p1;
}

void Triangle::setP2(Point new_p2) {
    this->p2 = new_p2;
}

Point Triangle::getP0() const {
    return p0;
}