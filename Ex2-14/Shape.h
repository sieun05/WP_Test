#pragma once

struct Shape {
	Shape() = default;
	Shape(int x, int y, int shape_sel, int r, int g, int b)
		: x{ x }, y{ y }, shape_sel{ shape_sel }, r{ r }, g{ g }, b{ b } {}

	int x{}, y{};
	int shape_sel{}; //1>�� 2>�ﰢ�� 3>�簢��
	int size{};
	int r{}, g{}, b{};
	int flag_c{};
};