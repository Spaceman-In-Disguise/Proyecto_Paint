#pragma once
#include <vector>
#include <memory>
#include "shape.h"
#include "engine2D.h" // For Color

class EditorMemento {
    //Private and immutable
    std::vector<std::unique_ptr<Shape>> savedShapes;
    Color savedBackgroundColor;

public:
    // The constructor takes ownership of the cloned shapes to store them securely
    EditorMemento(std::vector<std::unique_ptr<Shape>> shapes, Color bgColor)
        : savedShapes(std::move(shapes)), savedBackgroundColor(bgColor) {}

    // Getters for the Originator to use during restoration
    Color getBackgroundColor() const { return savedBackgroundColor; }

    // We return references to the stored unique_ptrs so the Originator can clone them back
    const std::vector<std::unique_ptr<Shape>>& getShapes() const { 
        return savedShapes; 
    }
};