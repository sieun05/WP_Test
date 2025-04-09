#pragma once
#include <Windows.h>

struct Shape {
	Shape() = default;
	Shape(int shape_sel, int x1, int y1, int x2, int y2, int width, int pen_color, int brush_color)
		: shape_sel{ shape_sel }, x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 }, width{ width }, pen_color{ pen_color }, brush_color{ brush_color } {
	}
	~Shape() = default;
	Shape(const Shape& s) {
		*this = s;
	}
	Shape& operator=(const Shape& s) {
		if (this == &s) return *this;
		
		shape_sel = s.shape_sel;
		x1={ s.x1 }, y1 = { s.y1 }, x2 = { s.x2 }, y2 = { s.y2 };
		width = s.width;
		pen_color = { s.pen_color }, brush_color = { s.brush_color };

		return *this;
	}

	int shape_sel{}; //2: 선 3: 삼각형 4: 사각형 5: 오각형 6: 육각형
	int x1, y1;
	int x2, y2;
	int width{};
	int size{};
	int pen_color{};
	int brush_color{};
};