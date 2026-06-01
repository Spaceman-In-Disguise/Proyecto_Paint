//
// Created by ManguitoRTX on 5/28/2026.
//

#include "triangle.h"

#include "line.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <utility>

namespace {
//Calculates arrays of x coordinates for each side
std::vector<int> interpolate(int i0, int d0, int i1, int d1) {
    //Same two points, no row
    if (i0 == i1) {
        return {d0};
    }

    //Number of rows to draw
    const int stepCount = i1 - i0;
    std::vector<int> values;
    values.reserve(stepCount + 1);

    //Slope or change rate of each step
    const float delta = static_cast<float>(d1 - d0) / static_cast<float>(stepCount);
    //Calculates each x positions taking the delta step
    for (int i = 0; i <= stepCount; ++i) {
        values.push_back(static_cast<int>(std::lround(d0 + (delta * i))));
    }

    return values;
}

//Draws the horizontal line for the scanline algorithm
void fillSpan(int y, int x0, int x1, const Color& color, const Triangle::PixelCallback& pixelWriter) {
    if (x0 > x1) {
        std::swap(x0, x1);
    }

    for (int x = x0; x <= x1; ++x) {
        pixelWriter(x, y, color);
    }
}

}

Triangle::Triangle(Point p0, Point p1, Point p2, const Color& color, PixelCallback pixelWriter)
    : p0(p0)
    , p1(p1)
    , p2(p2)
    , canvas(pixelWriter)
    , pixelWriter(std::move(pixelWriter)) {
    borderColor = color;
    fillColor = color;
}

void Triangle::draw() {
    if (!pixelWriter) {
        return;
    }

    // Create local copies for the rendering math
    Point t0 = p0;
    Point t1 = p1;
    Point t2 = p2;

    // Sort the local copies by height
    if (t1.y < t0.y) {std::swap(t1, t0);}
    if (t2.y < t0.y) {std::swap(t2, t0);}
    if (t2.y < t1.y) {std::swap(t2, t1);}

    position = Point((t0.x + t1.x + t2.x) / 3, (t0.y + t1.y + t2.y) / 3);
    boundingBox[0] = Point(std::min(std::min(p0.x,p1.x), p2.x), t0.y);
    boundingBox[1] = Point(std::max(std::max(p0.x,p1.x), p2.y), t2.y);

    canvas = Canvas(pixelWriter);

    Line* edgeA = canvas.addLine(p0, borderColor);
    Line* edgeB = canvas.addLine(p1, borderColor);
    Line* edgeC = canvas.addLine(p2, borderColor);

    canvas.resizeLine(p1, edgeA);
    canvas.resizeLine(p2, edgeB);
    canvas.resizeLine(p0, edgeC);


    if (fill) {
        // Calculate the edges using the sorted LOCAL copies (t0, t1, t2)
        const std::vector<int> x01 = interpolate(t0.y, t0.x, t1.y, t1.x);
        const std::vector<int> x12 = interpolate(t1.y, t1.x, t2.y, t2.x);
        const std::vector<int> x02 = interpolate(t0.y, t0.x, t2.y, t2.x);

        // TOP of the triangle is FLAT
        if (t1.y == t0.y) {
            for (std::size_t i = 0; i < x02.size() && i < x12.size(); ++i) {
                fillSpan(t0.y + static_cast<int>(i), x02[i], x12[i], fillColor, pixelWriter);
            }
        // BOTTOM of the triangle is FLAT
        } else if (t1.y == t2.y) {
            for (std::size_t i = 0; i < x01.size() && i < x02.size(); ++i) {
                fillSpan(t0.y + static_cast<int>(i), x01[i], x02[i], fillColor, pixelWriter);
            }
        // Else the triangle is split in two
        } else {
            for (std::size_t i = 0; i < x01.size() && i < x02.size(); ++i) {
                fillSpan(t0.y + static_cast<int>(i), x01[i], x02[i], fillColor, pixelWriter);
            }

            const int lowerOffset = t1.y - t0.y;
            for (std::size_t i = 0; i < x12.size() && i + lowerOffset < x02.size(); ++i) {
                fillSpan(t1.y + static_cast<int>(i), x12[i], x02[i + lowerOffset], fillColor, pixelWriter);
            }
        }
    }
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