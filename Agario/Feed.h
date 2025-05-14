#pragma once

struct Feed {
	Feed() = default;
	Feed(int x, int y, int color, int size)
		: x{ x }, y{ y }, color{ color }, size{ size } {
		time = 0;
	};

	int x;
	int y;
	int color;
	int size;
	int time;
};