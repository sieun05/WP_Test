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
	int move_method{}; //�̵���� 1�¿� 2���� 3�밢�� 4���ڸ����� 5���ڸ�ũ�⺯ȭ 0���ΰ����������
	int size{};
	int order{}; //���ΰ��� 1 -> 234, ������ �� 0
	int color{};
};