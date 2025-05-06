#pragma once

struct Obstacle {
	Obstacle() = default;
	Obstacle(int x, int y, int color)
		:x{ x }, y{ y }, color{ color } {};

	int x;
	int y;
	int color; //0�̸� �⺻ 1~4 ����
};

struct Obstacle2 {
	double angle;
	int radius;
	int color;
};