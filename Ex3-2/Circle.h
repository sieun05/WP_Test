#pragma once
struct Circle {
	Circle() = default;
	Circle(int x, int y, int direction, int size, int order, int color)
		: x{ x }, y{ y }, direction{ direction }, size { size }, order{ order }, color{ color } {
		direction_x = 1;
		direction_y = 1;
		size_flag = 1;
	}
	~Circle() = default;
	Circle(const Circle& c) = default;
	Circle& operator=(const Circle& c) = default;

	int x{}, y{};
	int direction{}; //+++ 2대각선 3제자리크기변화 0제자리정지
	int direction_x{1}; //1오른쪽 -1왼쪽
	int direction_y{1}; //1아래쪽 -1위쪽
	int size_flag{}; //1커짐 -1작아짐 0그대로
	int size{};
	int order{}; //주인공원 1 -> 234, 떨어진 원 0
	int color{};
};