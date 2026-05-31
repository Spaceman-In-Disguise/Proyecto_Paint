//
// Created by ManguitoRTX on 5/30/2026.
//

#ifndef PROYECTO_1_QUADTREE_H
#define PROYECTO_1_QUADTREE_H
#include <vector>

#include "quadNode.h"
#include "Shape.h"
#include "Engine2D.h"



class quadTree {
    public:
    using PixelCallback = std::function<void(int, int, const Color&)>;
    quadTree();
    ~quadTree();
    void insert(Point point);
    std::vector<Shape*> getShapesByLeaf(Point point);
    private:
    quadNode rootNode;
    Canvas canvas;
    PixelCallback pixelWriter;

};


#endif //PROYECTO_1_QUADTREE_H
