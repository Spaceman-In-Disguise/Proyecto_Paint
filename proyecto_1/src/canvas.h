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
class Ellipse;
class Bezier;

class Canvas {
    public:

    using PixelCallback = std::function<void(int, int, const Color&)>;

    explicit Canvas(PixelCallback pixelWriter);

    //Add Shape receiving its Pointer
    Shape* addShape(std::unique_ptr<Shape> newShape);
    [[nodiscard]] Shape* getLastShape() const;

    Shape* selectShape();
    static std::vector<Point> getControlPoints(Shape* shape);

    static Point getMiddlePoint(Shape* shape) ;

    void drawControlPoints(Shape* shape);

    void highLightShape(Shape* shape) const;

    Line* addLine(Point p0, Color color);
    static void resizeLine(Point p1, Shape* line);

    Rectangle* addRectangle(Point p0, bool fill, Color borderColor, Color fillColor);
    static void resizeRectangle(Point p1, Shape* rectangle);

    Triangle* addTriangle(Point p0, bool fill, Color borderColor, Color fillColor);
    static void resizeTriangle(Point p1, Shape* triangle);

    Ellipse* addEllipse(Point p0, bool fill, Color borderColor, Color fillColor);
    static void resizeEllipse(Point p1, Shape* ellipse);

    Bezier* addBezier(Point p, Color color);
    Bezier* addBezier(const std::vector<Point>& points, Color color);

    static void resizeBezier(Point p1, Shape* bezier);

    std::vector<Shape*> getShapes() const;

    void setShapes(std::vector<std::unique_ptr<Shape>> newShapes);

    void draw();

    void moveUp(Shape* shape);
    void moveDown(Shape* shape);
    void moveTop(Shape* shape);
    void moveBottom(Shape* shape);
    void deleteShape(Shape* shape);
    void clearAll();

    private:
    std::vector<std::unique_ptr<Shape>> shapes;
    PixelCallback pixelWriter;

};


#endif //PROYECTO_1_CANVAS_H
