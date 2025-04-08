#pragma once

struct Player {
	Player() = default;
	Player(int x, int y)
		: x{ x }, y{ y } {}

	
	~Player() = default;
	Player(const Player& p) {
		*this = p;
	}
	Player& operator=(const Player& p) {
		if (this == &p) {
			return *this;
		}
		x = p.x;
		y = p.y;

		return *this;
	}

	int x{ 10 }, y{ 19 };
};