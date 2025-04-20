#pragma once

struct Circle {
	Circle() = default;
	Circle(int x, int y, int direction, int size, int color)
		: x{ x }, y{ y }, direction{ direction }, size{ size }, color{ color } {
		direction_x = 1;
		direction_y = 1;
		size_flag = 1;
		parent_index = -1;
		prev_x = x;
		prev_y = y;
	}
	~Circle() = default;
	Circle(const Circle& c) = default;
	Circle& operator=(const Circle& c) = default;

	int x{}, y{};
	int prev_x{}, prev_y{};
	int direction{};
	int direction_x{ 1 };
	int direction_y{ 1 };
	int size_flag{};
	int size{};
	int color{};
	int parent_index{ -1 }; // -1 means main circle
};

