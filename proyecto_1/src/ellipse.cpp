//
// Created by ManguitoRTX on 5/28/2026.
//

#include "ellipse.h"

#include <cmath>
#include <algorithm>
#include <utility>

namespace {

void plotSymmetricPoints(const Ellipse::PixelCallback& pixelWriter, const Point& center, int x, int y, const Color& color) {
    pixelWriter(center.x + x, center.y + y, color);
    pixelWriter(center.x - x, center.y + y, color);
    pixelWriter(center.x + x, center.y - y, color);
    pixelWriter(center.x - x, center.y - y, color);
}

}

Ellipse::Ellipse(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0)
    , p1(p1)
    , color(color)
    , pixelWriter(std::move(pixelWriter)) {}

void Ellipse::draw() {
    if (!pixelWriter) {
        return;
    }

    const int left = std::min(p0.x, p1.x);
    const int right = std::max(p0.x, p1.x);
    const int top = std::min(p0.y, p1.y);
    const int bottom = std::max(p0.y, p1.y);

    const Point center(
        static_cast<int>(std::lround((left + right) * 0.5)),
        static_cast<int>(std::lround((top + bottom) * 0.5))
    );

    position = center;
    boundingBox[0] = Point(left, top);
    boundingBox[1] = Point(right, bottom);

    const int rx = (right - left) / 2;
    const int ry = (bottom - top) / 2;

    if (rx == 0 && ry == 0) {
        pixelWriter(center.x, center.y, color);
        return;
    }

    if (rx == 0) {
        for (int y = center.y - ry; y <= center.y + ry; ++y) {
            pixelWriter(center.x, y, color);
        }
        return;
    }

    if (ry == 0) {
        for (int x = center.x - rx; x <= center.x + rx; ++x) {
            pixelWriter(x, center.y, color);
        }
        return;
    }

    const long long rx2 = static_cast<long long>(rx) * rx;
    const long long ry2 = static_cast<long long>(ry) * ry;
    const long long twoRx2 = 2 * rx2;
    const long long twoRy2 = 2 * ry2;

    long long x = 0;
    long long y = ry;

    long long px = 0;
    long long py = twoRx2 * y;

    long long decision = ry2 - (rx2 * ry) + (rx2 / 4);

    auto drawHorizontalSpans = [&](const int currentX, const int currentY) {
        for (int dx = -currentX; dx <= currentX; ++dx) {
            pixelWriter(center.x + dx, center.y + currentY, color); // Lower half
            if (currentY != 0) {
                pixelWriter(center.x + dx, center.y - currentY, color); // Upper half
            }
        }
    };

    while (px < py) {
        plotSymmetricPoints(pixelWriter, center, static_cast<int>(x), static_cast<int>(y), color);
        x++;
        px += twoRy2;

        if (decision < 0) {
            decision += ry2 + px;
        } else {

            if (fill) {drawHorizontalSpans(static_cast<int>(x - 1), static_cast<int>(y-1));}
            y--;
            py -= twoRx2;
            decision += ry2 + px - py;
        }
    }

    decision = static_cast<long long>(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);

    while (y >= 0) {
        if (fill){drawHorizontalSpans(static_cast<int>(x), static_cast<int>(y));}
        plotSymmetricPoints(pixelWriter, center, static_cast<int>(x), static_cast<int>(y), color);
        y--;
        py -= twoRx2;

        if (decision > 0) {
            decision += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            decision += rx2 - py + px;
        }
    }
}

void Ellipse::setP1(Point new_p1) {
    this->p1 = new_p1;
}