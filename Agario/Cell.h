#pragma once
//struct Cell {
//	Cell() = default;
//	Cell(int x, int y, int size, float speed)
//		: x{ x }, y{ y }, size{ size }, speed{ speed } {};
//	~Cell() = default;
//	Cell(const Cell& c) = default;
//	Cell& operator=(const Cell& c) = default;
//
//	int x;
//	int y;
//	int size;
//	float speed;
//};

struct Cell {
	int x, y;
	float fx, fy; // 정밀 이동용
	int size;
	float speed;

	Cell(int x, int y, int size, float speed)
		: x{ x }, y{ y }, fx{ (float)x }, fy{ (float)y }, size{ size }, speed{ speed } {}
};
