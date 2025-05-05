#pragma once

struct Car {
	Car() = default;
	Car(int direction, int position, int speed) : direction{ direction }, position{ position }, speed{ speed } {
		move_flag = false;
	}

	int direction; // �¿� 1 -1 ���Ʒ� 2 -2
	int position;
	int speed;
	bool move_flag;
};