#pragma once
#include <windows.h>
#include <time.h>

struct Player {
	Player() = default;
	Player(int x, int y) : x{ x }, y{ y } {}
	Player(int x, int y, int size) : x{ x }, y{ y }, size(size) {}

	int x{}, y{};
	int size{ -2 };
	int shape{}; //0 (�⺻ ��), 1 (�ﰢ��), 2 (������), 3(������)
	int r{255}, g{255}, b{255}; //0 (����), 1 (�Ķ�), 2 (�ʷ�), 3 (���)
};
