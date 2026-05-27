//
// Created by ManguitoRTX on 5/26/2026.
// Here the shapes are stored and managed
//

#include "canvas.h"


void Canvas::addShape(std::unique_ptr<Shape> newShape) {
    shapes.push_back(std::move(newShape));
}

Shape* Canvas::getLastShape() {
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
