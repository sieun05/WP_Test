#pragma once

struct Car {
	Car() = default;
	Car(int direction, int position, int speed) : direction{ direction }, position{ position }, speed{ speed } {
		move_flag = false;
	}

	int direction; // ÁÂ¿ì 1 -1 À§¾Æ·¡ 2 -2
	int position;
	int speed;
	bool move_flag;
};