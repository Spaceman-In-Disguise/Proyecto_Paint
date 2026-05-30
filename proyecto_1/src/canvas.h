//
// Created by ManguitoRTX on 5/26/2026.
//

#ifndef PROYECTO_1_CANVAS_H
#define PROYECTO_1_CANVAS_H

#include "engine2D.h"
#include "Shape.h"
#include <functional>
#include <memory>

class Line;
class Rectangle;
class Triangle;
class Circle;

class Canvas {
    public:

    using PixelCallback = std::function<void(int, int, const Color&)>;

    explicit Canvas(PixelCallback pixelWriter);

    //Add Shape receiving its Pointer
    Shape* addShape(std::unique_ptr<Shape> newShape);
    [[nodiscard]] Shape* getLastShape() const;

    Line* addLine(Point p0, Color colorPincel);
    static void resizeLine(Point p1, Shape* line);

    Rectangle* addRectangle(Point p0, bool fill, Color colorPincel);
    static void resizeRectangle(Point p1, Shape* rectangle);

    Triangle* addTriangle(Point p0, bool fill, Color colorPincel);
    static void resizeTriangle(Point p1, Shape* triangle);

    Circle* addCircle(Point p0, Color colorPincel);
    static void resizeCircle(Point p1, Shape* circle);

    void draw();
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    PixelCallback pixelWriter;

};


#endif //PROYECTO_1_CANVAS_H
