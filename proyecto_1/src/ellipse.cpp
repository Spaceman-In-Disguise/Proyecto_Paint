//
// Created by ManguitoRTX on 5/28/2026.
//

#include "ellipse.h"

#include <cmath>
#include <utility>

Ellipse::Ellipse(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0)
    , p1(p1)
    , color(color)
    , pixelWriter(std::move(pixelWriter)) {}

void Ellipse::draw() {
    if (!pixelWriter) {
        return;
    }

    const int dx = p1.x - p0.x;
    const int dy = p1.y - p0.y;
    const int radius = static_cast<int>(std::lround(std::sqrt(static_cast<double>(dx * dx + dy * dy))));

    int x = 0;
    int y = radius;
    int decision = 3 - 2 * radius;

    auto plotSymmetricPoints = [this](int offsetX, int offsetY) {
        pixelWriter(p0.x + offsetX, p0.y + offsetY, color);
        pixelWriter(p0.x - offsetX, p0.y + offsetY, color);
        pixelWriter(p0.x + offsetX, p0.y - offsetY, color);
        pixelWriter(p0.x - offsetX, p0.y - offsetY, color);
        pixelWriter(p0.x + offsetY, p0.y + offsetX, color);
        pixelWriter(p0.x - offsetY, p0.y + offsetX, color);
        pixelWriter(p0.x + offsetY, p0.y - offsetX, color);
        pixelWriter(p0.x - offsetY, p0.y - offsetX, color);
    };

    while (y >= x) {
        plotSymmetricPoints(x, y);
        x++;

        if (decision > 0) {
            y--;
            decision += 4 * (x - y) + 10;
        } else {
            decision += 4 * x + 6;
        }
    }
}

void Ellipse::setP1(Point new_p1) {
    this->p1 = new_p1;
}