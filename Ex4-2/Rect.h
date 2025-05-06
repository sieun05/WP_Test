#pragma once
#include <Windows.h>

struct Rect {
	Rect() = default;
	Rect(int sx, int sy, int ex, int ey, int color, int border)
		:sx{ sx }, sy{ sy }, ex{ ex }, ey{ ey }, color{ color }, border{ border } {}

	int sx, sy, ex, ey;
	int color;
	int border;
	int move_flag;
	int DB_click_flag;
};