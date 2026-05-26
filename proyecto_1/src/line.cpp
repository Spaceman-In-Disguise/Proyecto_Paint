#include "line.h"

#include <cstdlib>

Line::Line(int x0, int y0, int x1, int y1, const Color& color, PixelCallback pixelWriter)
    : x0(x0), y0(y0), x1(x1), y1(y1), color(color), pixelWriter(std::move(pixelWriter)) {}

void Line::draw() {
    int dx = std::abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;

    int x = x0;
    int y = y0;

    while (true) {
        pixelWriter(x, y, color);

        if (x == x1 && y == y1) {
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
