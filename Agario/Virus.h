#pragma once
#include <random>

#define PI 3.141592
#define DEG2RAD(angle) ((angle) * PI / 180.0)
#define RAD2DEG(angle) ((angle) * 180.0 / PI)

struct Virus {
	Virus() = default;
	Virus(int x, int y, int color)
		: x{ x }, y{ y }, color{ color }, DESC_flag{ false }, ASC_num{ 0 }, size{ 20 }, speed{ (float)60 / size }, DESC_cnt { 0 }
	{
	};
	Virus(int x, int y, int size, int color, bool DESC_flag, int ASC_num, int DESC_order)
		: x{ x }, y{ y }, size{ size }, color{ color }, 
		DESC_flag{ DESC_flag }, ASC_num{ ASC_num }, DESC_order{ DESC_order }, speed { (float)60 / size }
	{
	};

	int x;
	int y;
	int size;
	int color;
	float speed;

	bool go_flag{};
	bool go_feed_index{};

	int DESC_cnt{};
	int DESC_order{};
	int ASC_num;
	bool DESC_flag;
	int tick{};
	int feed_tick{};
};