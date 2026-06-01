//
// Created by ManguitoRTX on 5/30/2026.
//

#ifndef PROYECTO_1_QUADTREE_H
#define PROYECTO_1_QUADTREE_H
#include <functional>
#include <vector>

#include "quadNode.h"
#include "Shape.h"

class quadTree {
    public:
    using PixelCallback = std::function<void(int, int, const Color&)>;
    quadTree(Point topLeft, Point bottomRight, PixelCallback pixelWriter);
    ~quadTree();
    void rebuild(const std::vector<Shape*>& shapes); //Each time a shape is added or modified we clear the tree and insert all the shapes again
    void insert(Shape* shape);
    void draw() const; //Draws the mesh of the quadTree for debugging purposes
    std::vector<Shape*> getShapesByLeaf(Point point) const; //Takes a point from the mouse position and returns the shapes from that leaf
    quadNode* getRoot() const;

    private:
    void resetRoot();

    Point boundingBox[2];
    quadNode* rootNode = nullptr;
    PixelCallback pixelWriter;

};


#endif //PROYECTO_1_QUADTREE_H
