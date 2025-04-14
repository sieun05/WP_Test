#pragma once
struct Circle {
	Circle() = default;
	Circle(int x, int y, int size, int order, int color)
		: x{ x }, y{ y }, size{ size }, order{ order }, color{ color } {
	}
	~Circle() = default;
	Circle(const Circle& c) {
		*this = c;
	}
	Circle& operator=(const Circle& c) {
		if (this == &c) return *this;

		x = { c.x }, y = { c.y }, size = { c.size }, order = { c.order }, color={ c.color };
		return *this;
	}

	int x{}, y{};
	int size{};
	int order{};
	int color{};
};