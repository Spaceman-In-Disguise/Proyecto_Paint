#pragma once

#include "Shape.h"
#include <functional>

class Line : public Shape {
public:
	using PixelCallback = std::function<void(int, int, const Color&)>;

	Line(Point p0, Point p1, const Color& color, PixelCallback pixelWriter);

	void draw() override;

private:
	Point p0;
	Point p1;
	Color color;
	PixelCallback pixelWriter;
};
