#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "line.h"
#include "rectangle.h"
#include "triangle.h"
#include "ellipse.h"
#include "bezier.h"

enum class SavedShapeType {
    Line,
    Rectangle,
    Triangle,
    Ellipse,
    Bezier
};

struct SavedShape {
    SavedShapeType type;
    bool fill = true;
    Color borderColor;
    Color fillColor;
    std::vector<Point> points;
};

class SaveManager {
public:
    static bool savePainting(const std::string& filePath, const std::vector<Shape*>& shapes, const Color& backgroundColor);
    static bool loadPainting(const std::string& filePath, std::vector<SavedShape>& outShapes, Color& outBackgroundColor);

private:
    static bool readColor(std::istream& stream, Color& color);
    static bool readPoint(std::istream& stream, Point& point);
    static const char* shapeTypeToString(SavedShapeType type);
    static bool stringToShapeType(const std::string& token, SavedShapeType& type);
};