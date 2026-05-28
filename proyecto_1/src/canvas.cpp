//
// Created by ManguitoRTX on 5/26/2026.
// Here the shapes are stored and managed
//

#include "canvas.h"
#include  "line.h"


void Canvas::addShape(std::unique_ptr<Shape> newShape) {
    shapes.push_back(std::move(newShape));
}

Shape* Canvas::getLastShape() const {
    if (shapes.empty()) {
        return nullptr;
    }
    return shapes.back().get();
}

void Canvas::draw() {
    std::cout << &pixelWriter << std::endl;
    //Iterate through all and render using the draw method
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

void Canvas::addLine(const int x, const int y, Color colorPincel) {
        addShape(std::make_unique<Line>(
            Point(x,y),
            Point(x,y),
            colorPincel,
            pixelWriter
        ));
}

void Canvas::resizeLine(const int x, const int y, Shape* line) {
    if (const auto latestLine = dynamic_cast<Line*>(line); latestLine != nullptr) {
        latestLine->setP1(Point(x, y));
    }
}