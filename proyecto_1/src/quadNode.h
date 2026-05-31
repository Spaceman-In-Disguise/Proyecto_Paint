//
// Created by ManguitoRTX on 5/30/2026.
//

#ifndef PROYECTO_1_QUADNODE_H
#define PROYECTO_1_QUADNODE_H
#include "engine2D.h"
#include "rectangle.h"


class quadNode {

public:
    quadNode(Point boundingBox[2], int level);
    ~quadNode();

private:
    Point boundingBox[2];
    const int MAX_LEVEL = 5;
    const int MAX_ELEMENTS = 3;
    int level;
    bool isLeaf = false;
    int numElements = 0;
    quadNode *children[4] = {nullptr, nullptr, nullptr, nullptr};

    std::vector<Shape*> shapes;
    void divideNode();
    bool isInside(Point shapeBox[2]) const;
};


#endif //PROYECTO_1_QUADNODE_H
