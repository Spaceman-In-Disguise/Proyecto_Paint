#pragma once

#include "Shape.h"
#include <functional>

class Line : public Shape {
public:
	using PixelCallback = std::function<void(int, int, const Color&)>;

	Line(Point p0, Point p1, const Color& color, PixelCallback pixelWriter);

	void draw() override;
	void setP1(Point new_p1);
private:
	Point p0;
	Point p1;
	Color color;
	PixelCallback pixelWriter;
};
