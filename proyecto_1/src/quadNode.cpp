//
// Created by ManguitoRTX on 5/30/2026.
//

#include "quadNode.h"
#include <stdexcept>
#include <algorithm>

namespace {
void drawHorizontalLine(const quadNode::PixelCallback& pixelWriter, const Point& left, const Point& right) {
    if (left.y != right.y) {
        return;
    }

    const int startX = std::min(left.x, right.x);
    const int endX = std::max(left.x, right.x);

    for (int x = startX; x <= endX; ++x) {
        pixelWriter(x, left.y, Color(0.0f, 1.0f, 0.0f));
    }
}

void drawVerticalLine(const quadNode::PixelCallback& pixelWriter, const Point& top, const Point& bottom) {
    if (top.x != bottom.x) {
        return;
    }

    const int startY = std::min(top.y, bottom.y);
    const int endY = std::max(top.y, bottom.y);

    for (int y = startY; y <= endY; ++y) {
        pixelWriter(top.x, y, Color(0.0f, 1.0f, 0.0f));
    }
}
}

quadNode::quadNode(Point boundingBox[2], int level) {
    this->boundingBox[0] = boundingBox[0];
    this->boundingBox[1] = boundingBox[1];
    this->level = level;
    this->isLeaf = true;
}

void quadNode::divideNode() {
    if (level >= MAX_LEVEL) {
        throw std::out_of_range("quadNode::divideNode");
    }

    const int minX = boundingBox[0].x;
    const int minY = boundingBox[0].y;
    const int maxX = boundingBox[1].x;
    const int maxY = boundingBox[1].y;

    const int midX = (minX + maxX) / 2;
    const int midY = (minY + maxY) / 2;

    // child 0: top-left
    Point cb0[2] = { Point(minX, minY), Point(midX, midY) };
    // child 1: top-right
    Point cb1[2] = { Point(midX, minY), Point(maxX, midY) };
    // child 2: bottom-left
    Point cb2[2] = { Point(minX, midY), Point(midX, maxY) };
    // child 3: bottom-right
    Point cb3[2] = { Point(midX, midY), Point(maxX, maxY) };

    children[0] = new quadNode(cb0, level + 1);
    children[1] = new quadNode(cb1, level + 1);
    children[2] = new quadNode(cb2, level + 1);
    children[3] = new quadNode(cb3, level + 1);

    isLeaf = false;
}

void quadNode::insert(Shape* shape) {
    if (shape == nullptr || !isInside(shape->boundingBox)) {
        return;
    }

    if (isLeaf) {
        if (numElements < MAX_ELEMENTS || level >= MAX_LEVEL) {
            shapes.push_back(shape);
            ++numElements;
            return;
        }

        divideNode();

        const std::vector<Shape*> currentShapes = shapes;
        shapes.clear();
        numElements = 0;

        for (Shape* currentShape : currentShapes) {
            insert(currentShape);
        }
    }

    bool insertedIntoChild = false;
    for (int i = 0; i < 4; ++i) {
        if (children[i] != nullptr && children[i]->isInside(shape->boundingBox)) {
            children[i]->insert(shape);
            insertedIntoChild = true;
        }
    }

    if (!insertedIntoChild) {
        shapes.push_back(shape);
        ++numElements;
    }
}

const quadNode* quadNode::findLeaf(const Point& point) const {
    if (!containsPoint(point)) {
        return nullptr;
    }

    if (isLeaf) {
        return this;
    }

    const int midX = (boundingBox[0].x + boundingBox[1].x) / 2;
    const int midY = (boundingBox[0].y + boundingBox[1].y) / 2;

    int childIndex = 0;
    if (point.x >= midX) {
        childIndex += 1;
    }
    if (point.y >= midY) {
        childIndex += 2;
    }

    if (children[childIndex] == nullptr) {
        return this;
    }

    return children[childIndex]->findLeaf(point);
}

quadNode* quadNode::findLeaf(const Point& point) {
    return const_cast<quadNode*>(static_cast<const quadNode*>(this)->findLeaf(point));
}

void quadNode::drawBorder(const PixelCallback& pixelWriter) const {
    const Point topLeft = boundingBox[0];
    const Point bottomRight = boundingBox[1];
    const Point topRight(bottomRight.x, topLeft.y);
    const Point bottomLeft(topLeft.x, bottomRight.y);

    drawHorizontalLine(pixelWriter, topLeft, topRight);
    drawHorizontalLine(pixelWriter, bottomLeft, bottomRight);
    drawVerticalLine(pixelWriter, topLeft, bottomLeft);
    drawVerticalLine(pixelWriter, topRight, bottomRight);
}

void quadNode::drawMesh(const PixelCallback& pixelWriter) const {
    drawBorder(pixelWriter);

    for (const quadNode* child : children) {
        if (child != nullptr) {
            child->drawMesh(pixelWriter);
        }
    }
}

std::vector<Shape*> quadNode::getShapes() const {
    return shapes;
}

std::size_t quadNode::getShapeCount() const {
    return shapes.size();
}

bool quadNode::fullyContains(Point shapeBox[2]) const {
    return shapeBox[0].x >= boundingBox[0].x &&
           shapeBox[0].y >= boundingBox[0].y &&
           shapeBox[1].x <= boundingBox[1].x &&
           shapeBox[1].y <= boundingBox[1].y;
}

bool quadNode::containsPoint(const Point& point) const {
    return point.x >= boundingBox[0].x && point.x <= boundingBox[1].x &&
           point.y >= boundingBox[0].y && point.y <= boundingBox[1].y;
}

quadNode::~quadNode() {
    for (int i = 0; i < 4; ++i) {
        delete children[i];
        children[i] = nullptr;
    }
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