#pragma once

struct Virus {
	Virus() = default;
	Virus(int x, int y, int color, int direction_x, int direction_y, bool DESC_flag)
		: x{ x }, y{ y }, color{ color }, direction_x{ direction_x }, direction_y{ direction_y }, DESC_flag{ DESC_flag } 
	{
		size = 20;
		speed = (float)60 / size;
	};

	int x;
	int y;
	int size;
	int color;
	float speed;
	int direction_x;
	int direction_y;

	bool DESC_flag;
};