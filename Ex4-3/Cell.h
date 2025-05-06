#pragma once

#define ROW 20
#define COL 20

struct Cell {
	Cell() = default;
	Cell(int type)
		: type{ type } {
	}

	int hint_flag{ false };
	int open_flag{ false };
	int type; //1-지뢰 2-아이템 34567
	int piece;
};