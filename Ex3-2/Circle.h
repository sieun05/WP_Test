#pragma once
struct Circle {
	Circle() = default;
	Circle(int x, int y, int move_method, int size, int order, int color)
		: x{ x }, y{ y }, move_method{ move_method }, size { size }, order{ order }, color{ color } {
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
	int move_method{}; //이동방법 1좌우 2상하 3대각선 4제자리정지 5제자리크기변화 0주인공원지그재그
	int size{};
	int order{}; //주인공원 1 -> 234, 떨어진 원 0
	int color{};
};