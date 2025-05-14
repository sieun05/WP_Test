#pragma once

struct Cell {
	int x, y;
	float size;
	float speed;

	bool DESC_flag;
	int delay{};

	Cell(int x, int y, float size, float speed)
		: x{ x }, y{ y }, size{ size }, speed{ speed }, DESC_flag{ false } {}
	Cell(int x, int y, float size, float speed, bool DESC_flag)
		: x{ x }, y{ y }, size{ size }, speed{ speed }, DESC_flag{ DESC_flag } {}
};
