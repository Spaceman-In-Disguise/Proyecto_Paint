//
// Created by ManguitoRTX on 5/30/2026.
//

#ifndef PROYECTO_1_QUADNODE_H
#define PROYECTO_1_QUADNODE_H
#include <cstddef>
#include <functional>
#include "engine2D.h"
#include "rectangle.h"


class quadNode {

public:
    using PixelCallback = std::function<void(int, int, const Color&)>;

    quadNode(Point boundingBox[2], int level);
    ~quadNode();
    void divideNode();
    void insert(Shape* shape);
    quadNode* findLeaf(const Point& point);
    const quadNode* findLeaf(const Point& point) const;
    void drawMesh(const PixelCallback& pixelWriter) const;
    std::vector<Shape*> getShapes() const;
    std::size_t getShapeCount() const;
    bool isInside(Point shapeBox[2]) const;

private:
    bool fullyContains(Point shapeBox[2]) const;
    bool containsPoint(const Point& point) const;
    void drawBorder(const PixelCallback& pixelWriter) const;

    Point boundingBox[2];
    const int MAX_LEVEL = 5;
    const int MAX_ELEMENTS = 3;
    int level;
    bool isLeaf = true;
    int numElements = 0;
    quadNode *children[4] = {nullptr, nullptr, nullptr, nullptr};

    std::vector<Shape*> shapes;
};


#endif //PROYECTO_1_QUADNODE_H
