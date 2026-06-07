#include "bezier.h"
#include <cmath>
#include <algorithm>
#include <utility>

Bezier::Bezier(std::vector<Point> controlPoints, const Color& color, PixelCallback pixelWriter)
    : controlPoints(std::move(controlPoints)), pixelWriter(std::move(pixelWriter)) {
    borderColor = color;
    fillColor = color;
}

void Bezier::move(int deltaX, int deltaY) {
    for (auto& p : controlPoints) {
        p.x += deltaX;
        p.y += deltaY;
    }
}

void Bezier::setPoint(size_t index, Point p) {
    if (index < controlPoints.size()) {
        controlPoints[index] = p;
    }
}

void Bezier::addPoint(Point p) {
    controlPoints.push_back(p);
}

// Evaluate the N-degree curve at time t using De Casteljau's Algorithm
Point Bezier::evaluate(float t) const {
    if (controlPoints.empty()) return {0, 0};
    if (controlPoints.size() == 1) return controlPoints[0];

    std::vector<float> tempX(controlPoints.size());
    std::vector<float> tempY(controlPoints.size());
    
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        tempX[i] = static_cast<float>(controlPoints[i].x);
        tempY[i] = static_cast<float>(controlPoints[i].y);
    }

    // Recursively interpolate
    size_t n = controlPoints.size();
    for (size_t k = 1; k < n; ++k) {
        for (size_t i = 0; i < n - k; ++i) {
            tempX[i] = tempX[i] * (1.0f - t) + tempX[i + 1] * t;
            tempY[i] = tempY[i] * (1.0f - t) + tempY[i + 1] * t;
        }
    }

    return Point(
        static_cast<int>(std::round(tempX[0])),
        static_cast<int>(std::round(tempY[0]))
    );
}

void Bezier::draw() {
    if (!pixelWriter || controlPoints.size() < 2) return;

    int minX = controlPoints[0].x;
    int minY = controlPoints[0].y;
    int maxX = controlPoints[0].x;
    int maxY = controlPoints[0].y;

    for (const auto& p : controlPoints) {
        minX = std::min(minX, p.x);
        minY = std::min(minY, p.y);
        maxX = std::max(maxX, p.x);
        maxY = std::max(maxY, p.y);
    }

    boundingBox[0] = Point(minX, minY);
    boundingBox[1] = Point(maxX, maxY);
    position = Point((minX + maxX) / 2, (minY + maxY) / 2);

    // Dynamic resolution based on the bounding box size
    float dist = std::hypot(maxX - minX, maxY - minY);
    int segments = std::max(20, static_cast<int>(dist)); 
    float step = 1.0f / segments;

    Point prevPoint = controlPoints.front();

    for (int i = 1; i <= segments; ++i) {
        float t = i * step;
        Point currentPoint = evaluate(t);
        drawSegment(prevPoint, currentPoint);
        prevPoint = currentPoint;
    }
}

void Bezier::drawSegment(Point start, Point end) {
    int dx = std::abs(end.x - start.x);
    int sx = start.x < end.x ? 1 : -1;
    int dy = -std::abs(end.y - start.y);
    int sy = start.y < end.y ? 1 : -1;
    int error = dx + dy;

    int x = start.x;
    int y = start.y;

    while (true) {
        pixelWriter(x, y, borderColor);
        if (x == end.x && y == end.y) break;
        int e2 = 2 * error;
        if (e2 >= dy) { error += dy; x += sx; }
        if (e2 <= dx) { error += dx; y += sy; }
    }
}