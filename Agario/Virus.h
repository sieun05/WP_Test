#pragma once
#include <random>

struct Virus {
	Virus() = default;
	Virus(int x, int y, int color, bool DESC_flag, int ASC_num)
		: x{ x }, y{ y }, color{ color }, DESC_flag{ DESC_flag }, ASC_num{ ASC_num }
	{
		size = 20;
		speed = (float)40 / size;
		angle = RandomAngle();

	};

	int x;
	int y;
	int size;
	int color;
	float speed;
	int direction;
	float angle;
	int tick{};
	float target_angle;

	int ASC_num;
	bool DESC_flag;

	static float RandomAngle() {
		static std::mt19937 gen{ std::random_device{}() };
		static std::uniform_real_distribution<float> dist(0, 2.0f * 3.14159265f);
		return dist(gen);
	}
};