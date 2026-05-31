//
// Created by ManguitoRTX on 5/30/2026.
//

#include "quadNode.h"

quadNode::quadNode(Point boundingBox[2], int level) {
    this->boundingBox[0] = boundingBox[0];
    this->boundingBox[1] = boundingBox[1];
    this->level = level;
}

void quadNode::divideNode() {
    if (level >= MAX_ELEMENTS) {
        throw std::out_of_range("quadNode::divideNode");
    }
    /*
    children[0] = new quadNode();
    children[1] = new quadNode();
    children[2] = new quadNode();
    children[3] = new quadNode();
    */
}

bool quadNode::isInside(Point shapeBox[2]) const {
    if (shapeBox[1].x < boundingBox[0].x) {
        return false;
    }
    if (shapeBox[1].y < boundingBox[0].y) {
        return false;
    }
    if (shapeBox[0].x > boundingBox[1].x) {
        return false;
    }
    if (shapeBox[0].y > boundingBox[1].y) {
        return false;
    }
    return true;
}
