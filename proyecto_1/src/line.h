#pragma once

#include "engine2D.h"
#include <functional>

class Line {
public:
	using PixelCallback = std::function<void(int, int, const Color&)>;

	Line(int x0, int y0, int x1, int y1, const Color& color, PixelCallback pixelWriter);

	void draw();

private:
	int x0;
	int y0;
	int x1;
	int y1;
	Color color;
	PixelCallback pixelWriter;
};
