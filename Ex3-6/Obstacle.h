#pragma once

struct Obstacle {
	Obstacle() = default;
	Obstacle(int x, int y, int color)
		:x{ x }, y{ y }, color{ color } {};

	int x;
	int y;
	int color; //0이면 기본 1~4 랜덤
};

struct Obstacle2 {
	double angle;
	int radius;
	int color;
};