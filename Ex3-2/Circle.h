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
	int direction{}; //+++ 2�밢�� 3���ڸ�ũ�⺯ȭ 0���ڸ�����
	int direction_x{1}; //1������ -1����
	int direction_y{1}; //1�Ʒ��� -1����
	int size_flag{}; //1Ŀ�� -1�۾��� 0�״��
	int size{};
	int order{}; //���ΰ��� 1 -> 234, ������ �� 0
	int color{};
};