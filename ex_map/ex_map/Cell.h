#pragma once

#define MAP_WIDTH 50
#define MAP_HEIGHT 50
#define MAP_PIXEL_WIDTH MAP_WIDTH * 40
#define MAP_PIXEL_HEIGHT MAP_HEIGHT * 40

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 18

struct Cell {
	Cell() = default;
	Cell(int type) : type { type } {};
	Cell(int type, int block_num) : type{ type }, block_num{ block_num } {};
	Cell(const Cell& c) = default;
	Cell& operator=(const Cell& c) = default;

	int type;
	int block_num;
};