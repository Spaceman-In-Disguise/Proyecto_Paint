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
    , canvas(pixelWriter)
    , pixelWriter(std::move(pixelWriter)) {
    borderColor = color;
    fillColor = color;
}

void Rectangle::draw() {
    canvas = Canvas(pixelWriter);

    const int minX = std::min(p0.x, p1.x);
    const int maxX = std::max(p0.x, p1.x);
    const int minY = std::min(p0.y, p1.y);
    const int maxY = std::max(p0.y, p1.y);

    boundingBox[0] = Point(minX, minY);
    boundingBox[1] = Point(maxX, maxY);

    const Point topLeft = p0;
    const Point topRight(p1.x, p0.y);
    const Point bottomRight = p1;
    const Point bottomLeft(p0.x, p1.y);

    Line* topEdge = canvas.addLine(topLeft, borderColor);
    Line* leftEdge = canvas.addLine(bottomLeft, borderColor);
    canvas.resizeLine(topRight, topEdge);
    canvas.resizeLine(bottomRight, leftEdge);

    Line* bottomEdge = canvas.addLine(topLeft, borderColor);
    Line* rightEdge = canvas.addLine(topRight, borderColor);
    canvas.resizeLine(bottomLeft, bottomEdge);
    canvas.resizeLine(bottomRight, rightEdge);

    const int width = abs(bottomRight.x - topLeft.x);
    const int height = abs(bottomRight.y - topLeft.y);

    position = Point((minX + maxX) / 2, (minY + maxY) / 2);

    if (fill == true) {
        for (int i = 1; i < width; ++i) {
            for (int j = 1; j < height; ++j) {
                pixelWriter(fmin(p0.x,p1.x) + i, fmin(p0.y,p1.y) + j, fillColor);
            }
        }
    }

    canvas.draw();
}

void Rectangle::setP1(Point new_p1) {
    this->p1 = new_p1;
}

void Rectangle::setP0(Point new_p0) {
    this->p0 = new_p0;
}
