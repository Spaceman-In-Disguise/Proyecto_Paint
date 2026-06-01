//
// Created by ManguitoRTX on 5/30/2026.
//

#include "quadTree.h"
#include <utility>

quadTree::quadTree(Point topLeft, Point bottomRight, PixelCallback pixelWriter)
	: pixelWriter(std::move(pixelWriter)) {
	boundingBox[0] = topLeft;
	boundingBox[1] = bottomRight;
	resetRoot();
}

quadTree::~quadTree() {
	delete rootNode;
	rootNode = nullptr;
}

quadNode *quadTree::getRoot() const {
	return rootNode;
}

void quadTree::resetRoot() {
	delete rootNode;
	rootNode = new quadNode(boundingBox, 0);
}

void quadTree::rebuild(const std::vector<Shape*>& shapes) {
	resetRoot();

	for (Shape* shape : shapes) {
		insert(shape);
	}
}

void quadTree::insert(Shape* shape) {
	if (rootNode != nullptr) {
		rootNode->insert(shape);
	}
}

void quadTree::draw() const {
	if (rootNode != nullptr) {
		rootNode->drawMesh(pixelWriter);
	}
}

std::vector<Shape*> quadTree::getShapesByLeaf(Point point) const {
	if (rootNode == nullptr) {
		return {};
	}

	const quadNode* leaf = rootNode->findLeaf(point);
	if (leaf == nullptr) {
		return {};
	}

	return leaf->getShapes();
}