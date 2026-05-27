//
// Created by ManguitoRTX on 5/26/2026.
//

#ifndef PROYECTO_1_CANVAS_H
#define PROYECTO_1_CANVAS_H

#include "engine2D.h"
#include "Shape.h"
#include <memory>

class Canvas {
    public:
    //Add Shape recieving its Pointer
    void addShape(std::unique_ptr<Shape> newShape);
    void draw();
private:
    std::vector<std::unique_ptr<Shape>> shapes;

};


#endif //PROYECTO_1_CANVAS_H
