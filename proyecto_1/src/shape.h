#pragma once

#include "engine2D.h"
#include <memory>

class Shape {
public:

    Point position = Point(0, 0);
    Point boundingBox[2] = { Point(-1,-1), Point(-1,-1) };
    Color borderColor = Color();
    Color fillColor = Color();
    bool fill = true;
    virtual ~Shape() = default;

    virtual void draw() = 0;
    virtual void move(int deltaX, int deltaY) = 0;
    virtual std::unique_ptr<Shape> clone() const = 0; // Used for the Memento Pattern in undo-redo logic
};

template <typename Derived>
class CloneableShape : public Shape {
public:
    std::unique_ptr<Shape> clone() const override {
        // Since we need to clone from this interface, we cast the Shape to its derivate
        // With a smart pointer (*this) and so the clone() works for each shape
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};