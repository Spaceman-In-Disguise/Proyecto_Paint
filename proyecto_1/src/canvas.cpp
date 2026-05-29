//
// Created by ManguitoRTX on 5/26/2026.
// Here the shapes are stored and managed
//

#include "canvas.h"

#include "line.h"
#include "rectangle.h"

Canvas::Canvas(PixelCallback pixelWriter)
    : pixelWriter(std::move(pixelWriter)) {}


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

Rectangle* Canvas::addRectangle(const Point p0, Color colorPincel) {
    Shape* newShape = addShape(std::make_unique<Rectangle>(
        p0,
        p0,
        colorPincel,
        pixelWriter
        ));
    return dynamic_cast<Rectangle*>(newShape);
}

void Canvas::resizeRectangle(const Point p1, Shape* rectangle) {
    if (const auto latestRectangle = dynamic_cast<Rectangle*>(rectangle); latestRectangle != nullptr) {
        latestRectangle->setP1(p1);
    }
}