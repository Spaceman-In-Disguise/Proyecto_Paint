#pragma once

#ifndef PROYECTO_1_BEZIER_H
#define PROYECTO_1_BEZIER_H

#include "Shape.h"
#include <functional>
#include <vector>

class Bezier : public CloneableShape<Bezier> {
public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    // Accepts a vector of points. The degree is automatically points.size() - 1
    Bezier(std::vector<Point> controlPoints, const Color& color, PixelCallback pixelWriter);

    void draw() override;
    void move(int deltaX, int deltaY) override;
    
    // Manage control points dynamically
    void setPoint(size_t index, Point p);
    void addPoint(Point p);
    
    std::vector<Point> getPoints() const { return controlPoints; }
    size_t getDegree() const { return controlPoints.empty() ? 0 : controlPoints.size() - 1; }

private:
    std::vector<Point> controlPoints;
    PixelCallback pixelWriter;
    
    void drawSegment(Point start, Point end);
    
    // De Casteljau's algorithm to evaluate the curve at parameter t (0.0 to 1.0)
    Point evaluate(float t) const;
};

#endif //PROYECTO_1_BEZIER_H    