#include "line.h"

#include <cstdlib>

Line::Line(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0), p1(p1), color(color), pixelWriter(std::move(pixelWriter)) {}

void Line::draw() {
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
}
