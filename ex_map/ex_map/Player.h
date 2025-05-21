#pragma once
#include "Cell.h"

struct Player {

	Player() = default;
	Player(int x, int y, int speed)
		: x{ x }, y{ y }, speed{ speed } {
	};

	/*void Player_Move(int dx, int dy) {
		int newx = x + dx;
		int newy = y + dy;


		if (newx<0 or newx>MAP_PIXEL_WIDTH or newy<0 or newy>MAP_PIXEL_HEIGHT) {
			return;
		}

		x = newx;
		y = newy;
	}*/

	int x;
	int y;
	int speed;
};