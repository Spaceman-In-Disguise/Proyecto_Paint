#include "line.h"

#include <cstdlib>

Line::Line(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0), p1(p1), color(color), pixelWriter(std::move(pixelWriter)) {}

void Line::draw() {
    if (!pixelWriter) {
        return;
    }

    position = Point((p0.x + p1.x)/2, (p0.y + p1.y)/2); //Middle Point
    boundingBox[0] = Point(std::min(p0.x,p1.x), std::min(p0.y, p1.y));
    boundingBox[1] = Point(std::max(p0.x,p1.x), std::max(p0.y, p1.y));

    int dx = std::abs(p1.x - p0.x);
    int sx = p0.x < p1.x ? 1 : -1;
    int dy = -std::abs(p1.y - p0.y);
    int sy = p0.y < p1.y ? 1 : -1;
    int error = dx + dy;

    int x = p0.x;
    int y = p0.y;

    while (true) {
        pixelWriter(x, y, color);

        if (x == p1.x && y == p1.y) {
            break;
        }

        int doubledError = 2 * error;
        if (doubledError >= dy) {
            error += dy;
            x += sx;
        }
        if (doubledError <= dx) {
            error += dx;
            y += sy;
        }
    }
    if (debug == true) { // Draw Control Middle Point
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                pixelWriter(position.x + i, position.y + j, Color(0,1,0));
            }
        }
    }
}

void Line::setP1(Point new_p1) {
    this->p1 = new_p1;
}
