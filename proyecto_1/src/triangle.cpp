//
// Created by ManguitoRTX on 5/28/2026.
//

#include "triangle.h"

#include "line.h"

#include <cmath>
#include <algorithm>
#include <vector>
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
    , color(color)
    , canvas(pixelWriter)
    , pixelWriter(std::move(pixelWriter)) {}

void Triangle::draw() {
    if (!pixelWriter) {
        return;
    }

    //We sort the vertices by height starting with the top one as p0
    if (p1.y < p0.y) {std::swap(p1, p0);}
    if (p2.y < p0.y) {std::swap(p2, p0);}
    if (p2.y < p1.y) {std::swap(p2, p1);}

    canvas = Canvas(pixelWriter);

    Line* edgeA = canvas.addLine(p0, color);
    Line* edgeB = canvas.addLine(p1, color);
    Line* edgeC = canvas.addLine(p2, color);

    canvas.resizeLine(p1, edgeA);
    canvas.resizeLine(p2, edgeB);
    canvas.resizeLine(p0, edgeC);

    canvas.draw();

    if (fill) {
        //Calculates the x coordinates of the triangle edges
        const std::vector<int> x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
        const std::vector<int> x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
        const std::vector<int> x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

        //TOP of the triangle is FLAT
        if (p1.y == p0.y) {
            for (std::size_t i = 0; i < x02.size() && i < x12.size(); ++i) {
                fillSpan(p0.y + static_cast<int>(i), x02[i], x12[i], color, pixelWriter);
            }
        //BOTTOM of the triangle is FLAT
        } else if (p1.y == p2.y) {
            for (std::size_t i = 0; i < x01.size() && i < x02.size(); ++i) {
                fillSpan(p0.y + static_cast<int>(i), x01[i], x02[i], color, pixelWriter);
            }
        //Else the triangle is split in two
        } else {
            for (std::size_t i = 0; i < x01.size() && i < x02.size(); ++i) {
                fillSpan(p0.y + static_cast<int>(i), x01[i], x02[i], color, pixelWriter);
            }

            const int lowerOffset = p1.y - p0.y;
            for (std::size_t i = 0; i < x12.size() && i + lowerOffset < x02.size(); ++i) {
                fillSpan(p1.y + static_cast<int>(i), x12[i], x02[i + lowerOffset], color, pixelWriter);
            }
        }
    }
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