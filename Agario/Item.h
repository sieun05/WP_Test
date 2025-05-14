#pragma once
#include <cmath>
#include <random>

struct Item {
	Item() = default;
	Item(int x, int y)
		: x{ x }, y{ y }, posible{ false }, delay{ 0 }, speed{ 2 }, size{ 30 } {

		setRandomDirection();
	};

	void setRandomDirection() {
		static std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * 3.14159f);

		float angle = angle_dist(rng);
		dirX = cosf(angle);
		dirY = sinf(angle);
	}

	int x;
	int y;
	bool posible;
	int delay;
	int size;

	float dirX, dirY;
	int speed;

	int using_time{};
};