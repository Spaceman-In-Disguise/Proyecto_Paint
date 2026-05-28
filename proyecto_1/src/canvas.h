//
// Created by ManguitoRTX on 5/26/2026.
//

#ifndef PROYECTO_1_CANVAS_H
#define PROYECTO_1_CANVAS_H

#include "engine2D.h"
#include "Shape.h"
#include <functional>
#include <memory>

class Canvas {
    public:

    using PixelCallback = std::function<void(int, int, const Color&)>;

    //Add Shape receiving its Pointer
    void addShape(std::unique_ptr<Shape> newShape);
    [[nodiscard]] Shape* getLastShape() const;

    void addLine(int x, int y, Color colorPincel);
    static void resizeLine(int x, int y, Shape* line);

    void draw();
private:
    std::vector<std::unique_ptr<Shape>> shapes;
    PixelCallback pixelWriter;

};


#endif //PROYECTO_1_CANVAS_H
