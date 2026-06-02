//
// Created by ManguitoRTX on 5/26/2026.
// Here the shapes are stored and managed
//

#include "canvas.h"

#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"

#include <algorithm>
#include <cmath>
#include <utility>

Canvas::Canvas(PixelCallback pixelWriter)
    : pixelWriter(std::move(pixelWriter)) {}


std::vector<Shape*> Canvas::getShapes() const {
    std::vector<Shape*> result;
    result.reserve(shapes.size());

    for (const auto& shape : shapes) {
        result.push_back(shape.get());
    }

    return result;
}

void Canvas::setShapes(std::vector<std::unique_ptr<Shape>> newShapes) {
    shapes.clear();
    shapes = std::move(newShapes);
};

Shape* Canvas::addShape(std::unique_ptr<Shape> newShape) {
    shapes.push_back(std::move(newShape));
    return shapes.back().get();
}

Shape* Canvas::getLastShape() const {
    if (shapes.empty()) {
        return nullptr;
    }
    return shapes.back().get();
}

void Canvas::deleteShape(Shape* shape) {
    if (!shape) return;
    shapes.erase(
        std::remove_if(shapes.begin(), shapes.end(),
                       [shape](const std::unique_ptr<Shape>& p) { return p.get() == shape; }),
        shapes.end()
    );
}

void Canvas::clearAll() {
    shapes.clear();
}

void Canvas::draw() {
    //Iterate through all and render using the draw method
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

std::vector<Point> Canvas::getControlPoints(Shape* shape) {
    // Return the control points based on the shape type
    if (auto line = dynamic_cast<Line*>(shape)) {
        return {line->getP0(), line->getP1()};
    } else if (auto rectangle = dynamic_cast<Rectangle*>(shape)) {
        return {rectangle->getP0(), rectangle->getP1()};
    } else if (auto triangle = dynamic_cast<Triangle*>(shape)) {
        return {triangle->getP0(), triangle->getP1(), triangle->getP2()};
    } else if (auto ellipse = dynamic_cast<Ellipse*>(shape)) {
        return {ellipse->getP0(), ellipse->getP1()};
    }

    return {};
}

Point Canvas::getMiddlePoint(Shape* shape) {
    return shape ->position;
}

void Canvas::drawControlPoints(Shape *shape) {
    std::vector<Point> controlPoints = getControlPoints(shape);
    controlPoints.push_back(getMiddlePoint(shape));

    //Draw a small square with a checkker pattern
    for (const auto& point : controlPoints) {
        for (int x = point.x - 3; x <= point.x + 3; ++x) {
            for (int y = point.y - 3; y <= point.y + 3; ++y) {
                const auto color = (x + y) % 2 == 0 ? Color(1, 0, 0) : Color(0, 0, 0);
                pixelWriter(x, y, color);
            }
        }
    }


}

void Canvas::highLightShape(Shape *shape) const {
    if (shape == nullptr) {
        return;
    }

    const auto green = Color(0, 1, 0);
    const auto black = Color(0, 0, 0);
    Color colorOptions[2] = {green, black};
    const int minX = std::min(shape->boundingBox[0].x, shape->boundingBox[1].x);
    const int maxX = std::max(shape->boundingBox[0].x, shape->boundingBox[1].x);
    const int minY = std::min(shape->boundingBox[0].y, shape->boundingBox[1].y);
    const int maxY = std::max(shape->boundingBox[0].y, shape->boundingBox[1].y);

    for (int x = minX; x <= maxX; ++x) {

        pixelWriter(x, minY, colorOptions[x%2]);
        pixelWriter(x, minY + 1, colorOptions[x%2]);
        pixelWriter(x, maxY, colorOptions[x%2]);
        pixelWriter(x, maxY - 1, colorOptions[x%2]);
    }

    for (int y = minY; y <= maxY; ++y) {
        pixelWriter(minX, y, colorOptions[y%2]);
        pixelWriter(minX + 1, y, colorOptions[y%2]);
        pixelWriter(maxX, y, colorOptions[y%2]);
        pixelWriter(maxX - 1, y, colorOptions[y%2]);
    }
}

Line* Canvas::addLine(const Point p0, Color color) {
        Shape* newShape = addShape(std::make_unique<Line>(
            p0,
            p0,
            color,
            pixelWriter
        ));
       newShape->borderColor = color;
       newShape->fillColor = color;
       return dynamic_cast<Line*>(newShape);
}

void Canvas::resizeLine(const Point p1, Shape* line) {
    if (const auto latestLine = dynamic_cast<Line*>(line); latestLine != nullptr) {
        latestLine->setP1(p1);
    }
}

Rectangle* Canvas::addRectangle(const Point p0, const bool fill, Color borderColor, Color fillColor) {
    Shape* newShape = addShape(std::make_unique<Rectangle>(
        p0,
        p0,
        borderColor,
        pixelWriter
        ));
    newShape->borderColor = borderColor;
    newShape->fillColor = fillColor;
    newShape -> fill = fill;
    return dynamic_cast<Rectangle*>(newShape);
}

void Canvas::resizeRectangle(const Point p1, Shape* rectangle) {
    if (const auto latestRectangle = dynamic_cast<Rectangle*>(rectangle); latestRectangle != nullptr) {
        latestRectangle->setP1(p1);
    }
}

Triangle* Canvas::addTriangle(const Point p0, const bool fill, Color borderColor, Color fillColor) {
    Shape* newShape = addShape(std::make_unique<Triangle>(
        p0,
        p0,
        p0,
        borderColor,
        pixelWriter
    ));
    newShape->borderColor = borderColor;
    newShape->fillColor = fillColor;
    newShape -> fill = fill;
    return dynamic_cast<Triangle*>(newShape);
}

void Canvas::resizeTriangle(const Point p1, Shape* triangle) {
    if (const auto latestTriangle = dynamic_cast<Triangle*>(triangle); latestTriangle != nullptr) {
        // Set the bottom-right point to the current mouse position
        latestTriangle->setP1(p1);

        const Point origin = latestTriangle->getP0();

        // Calculate the horizontal distance from the top center
        const int dx = p1.x - origin.x;

        // Mirror p1 across the origin's X-axis to find the bottom-left point.
        // Keep the Y exactly the same so the base is perfectly flat.
        const Point p2(
            origin.x - dx,
            p1.y
        );

        latestTriangle->setP2(p2);
    }
}

Ellipse* Canvas::addEllipse(const Point p0, const bool fill, Color borderColor, Color fillColor) {
    Shape* newShape = addShape(std::make_unique<Ellipse>(
        p0,
        p0,
        borderColor,
        pixelWriter
    ));
    newShape->borderColor = borderColor;
    newShape->fillColor = fillColor;
    newShape->fill = fill;
    return dynamic_cast<Ellipse*>(newShape);
}

void Canvas::resizeEllipse(const Point p1, Shape* ellipse) {
    if (const auto latestEllipse = dynamic_cast<Ellipse*>(ellipse); latestEllipse != nullptr) {
        latestEllipse->setP1(p1);
    }
}

void Canvas::moveUp(Shape* shape) {
    if (!shape) return;

    // Find the shape in the vector
    auto it = std::find_if(shapes.begin(), shapes.end(),
                           [shape](const std::unique_ptr<Shape>& p) { return p.get() == shape; });

    // If found and not already at the very end (top)
    if (it != shapes.end() && it != shapes.end() - 1) {
        // Swap with the next element
        std::iter_swap(it, it + 1);
    }
}

void Canvas::moveDown(Shape* shape) {
    if (!shape) return;

    auto it = std::find_if(shapes.begin(), shapes.end(),
                           [shape](const std::unique_ptr<Shape>& p) { return p.get() == shape; });

    // If found and not already at the very beginning (bottom)
    if (it != shapes.end() && it != shapes.begin()) {
        // Swap with the previous element
        std::iter_swap(it, it - 1);
    }
}

void Canvas::moveTop(Shape* shape) {
    if (!shape) return;

    auto it = std::find_if(shapes.begin(), shapes.end(),
                           [shape](const std::unique_ptr<Shape>& p) { return p.get() == shape; });

    if (it != shapes.end() && it != shapes.end() - 1) {
        // std::rotate shifts elements left
        // By rotating from 'it' to 'shapes.end()', 'it' gets pushed to the back
        std::rotate(it, it + 1, shapes.end());
    }
}

void Canvas::moveBottom(Shape* shape) {
    if (!shape) return;

    auto it = std::find_if(shapes.begin(), shapes.end(),
                           [shape](const std::unique_ptr<Shape>& p) { return p.get() == shape; });

    if (it != shapes.end() && it != shapes.begin()) {
        // Here, we rotate the segment from the beginning up to the element AFTER 'it'.
        // This effectively pushes 'it' to the front and shifts everything else right.
        std::rotate(shapes.begin(), it, it + 1);
    }
}