//
// Created by ManguitoRTX on 5/27/2026.
//
#pragma once
#include "rectangle.h"

#include <cstdlib>

Rectangle::Rectangle(Point p0, Point p1, const Color& color, PixelCallback pixelWriter)
    : p0(p0), p1(p1), color(color), pixelWriter(std::move(pixelWriter)) {}

void Rectangle::draw() {
    std::cout << "DRAWS RECTANGLE";
}
