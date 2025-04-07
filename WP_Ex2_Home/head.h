#pragma once
#include <windows.h>
#include <time.h>

struct Player {
	Player() = default;
	Player(int x, int y) : x{ x }, y{ y } {}
	Player(int x, int y, int size) : x{ x }, y{ y }, size(size) {}

	int x{}, y{};
	int size{ -2 };
	int shape{}; //0 (기본 원), 1 (삼각형), 2 (사작형), 3(마름모)
	int r{255}, g{255}, b{255}; //0 (빨강), 1 (파랑), 2 (초록), 3 (노랑)
};
