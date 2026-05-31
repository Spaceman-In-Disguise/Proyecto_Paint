//
// Created by ManguitoRTX on 5/30/2026.
//

#ifndef PROYECTO_1_QUADNODE_H
#define PROYECTO_1_QUADNODE_H
#include "engine2D.h"
#include "rectangle.h"


class quadNode {

public:
    using PixelCallback = std::function<void(int, int, const Color&)>;
    quadNode();
    ~quadNode();
    int maxElements = 3;

private:
    Point position;
    int numElements;
    std::vector<Shape*> shapes;
    void divideNode();
    int width;
    int height;
    int level;
    quadNode *children[4];
};


#endif //PROYECTO_1_QUADNODE_H
