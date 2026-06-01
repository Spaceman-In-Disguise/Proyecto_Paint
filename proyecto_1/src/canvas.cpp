//
// Created by ManguitoRTX on 5/26/2026.
// Here the shapes are stored and managed
//

#include "canvas.h"

#include "line.h"
#include "ellipse.h"
#include "rectangle.h"
#include "triangle.h"

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

void Canvas::draw() {
    //Iterate through all and render using the draw method
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

void Canvas::highLightShape(Shape *shape) const {

    const auto green = Color(0,1,0);
    const int width = shape->boundingBox[0].x - shape->boundingBox[1].x;
    const int height = shape->boundingBox[1].y - shape->boundingBox[0].y;

    for (int i = shape->boundingBox[0].x; i < width+1; ++i) {
        pixelWriter(i, shape->boundingBox[0].y, green);
        pixelWriter(i, shape->boundingBox[0].y+1, green);
        pixelWriter(i, shape->boundingBox[1].y, green);
        pixelWriter(i, shape->boundingBox[1].y-1, green);
    }
    for (int i = shape->boundingBox[0].y; i < height+1; ++i) {
        pixelWriter(shape->boundingBox[0].x, i, green);
        pixelWriter(shape->boundingBox[0].x+1, i, green);
        pixelWriter(shape->boundingBox[1].x, i, green);
        pixelWriter(shape->boundingBox[1].x-1, i, green);
    }
}

Line* Canvas::addLine(const Point p0, Color colorPincel) {
        Shape* newShape = addShape(std::make_unique<Line>(
            p0,
            p0,
            colorPincel,
            pixelWriter
        ));
       return dynamic_cast<Line*>(newShape);
}

void Canvas::resizeLine(const Point p1, Shape* line) {
    if (const auto latestLine = dynamic_cast<Line*>(line); latestLine != nullptr) {
        latestLine->setP1(p1);
    }
}

Rectangle* Canvas::addRectangle(const Point p0, const bool fill, Color colorPincel) {
    Shape* newShape = addShape(std::make_unique<Rectangle>(
        p0,
        p0,
        colorPincel,
        pixelWriter
        ));
    newShape -> fill = fill;
    return dynamic_cast<Rectangle*>(newShape);
}

void Canvas::resizeRectangle(const Point p1, Shape* rectangle) {
    if (const auto latestRectangle = dynamic_cast<Rectangle*>(rectangle); latestRectangle != nullptr) {
        latestRectangle->setP1(p1);
    }
}

Triangle* Canvas::addTriangle(const Point p0, const bool fill, Color colorPincel) {
    Shape* newShape = addShape(std::make_unique<Triangle>(
        p0,
        p0,
        p0,
        colorPincel,
        pixelWriter
    ));
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

Ellipse* Canvas::addEllipse(const Point p0, const bool fill, Color colorPincel) {
    Shape* newShape = addShape(std::make_unique<Ellipse>(
        p0,
        p0,
        colorPincel,
        pixelWriter
    ));
    newShape->fill = fill;
    return dynamic_cast<Ellipse*>(newShape);
}

void Canvas::resizeEllipse(const Point p1, Shape* ellipse) {
    if (const auto latestEllipse = dynamic_cast<Ellipse*>(ellipse); latestEllipse != nullptr) {
        latestEllipse->setP1(p1);
    }
}