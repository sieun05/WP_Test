#pragma once
#include <Windows.h>
#include <vector>
#include <random>
#include "GRID_PAINT.h"
#include "Cell.h"
#include "Shape.h"

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, ROW - 1); // 0 ~ 39
std::uniform_int_distribution<> block_r(1, 3);
std::uniform_int_distribution<> color_r(0, 5);

void Block_Move(Cell board[ROW][COLUMN], std::vector<Block>& blocks, int dx, int dy, bool& block_down_flag, bool& game_end_flag);
void Block_Create(Cell board[ROW][COLUMN], std::vector<Block>& blocks, bool& block_down_flag, bool& game_end_flag);
void Block_Check(Cell board[ROW][COLUMN]);
bool Block_4x4_Check(Cell board[ROW][COLUMN], int sy, int sx);



void Block_Move(Cell board[ROW][COLUMN], std::vector<Block>& blocks, int dx, int dy, bool& block_down_flag, bool& game_end_flag) {
	if (blocks.size() < 1 or game_end_flag) return;

	Block& b = blocks[blocks.size() - 1];
	
	int newX = b.x + dx;
	int newY = b.y + dy;

	bool flag_y{ false };
	for (int i = b.x; i < b.x + b.width; ++i) {
		for (int j = b.y + b.height - 1; j >= b.y; --j) {
			if (j + 1 >= ROW || board[j + 1][i].type) {
				flag_y = true;
				break;
			}
		}
		if (flag_y) {
			for (int i{ b.x }; i < b.x + b.width; i++) {
				for (int j{ b.y }; j < b.y + b.height; j++) {
					board[j][i].type = 1;
					board[j][i].color = b.color;
				}
			}
			block_down_flag = false;
			Block_Check(board);
			blocks.pop_back();
			Block_Create(board, blocks, block_down_flag, game_end_flag);
			return;
		}
	}

	bool flag_x = false;

	if (newX < 0 || newX + b.width > COLUMN) {
		flag_x = true; // 경계 벗어남
	}
	else {
		for (int j = b.y; j < b.y + b.height; ++j) {
			for (int i = 0; i < b.width; ++i) {
				if (board[j][newX + i].type) {
					flag_x = true; // 옆에 블럭 있음
					break;
				}
			}
			if (flag_x) break;
		}
	}
	if(flag_x) return;

	b.x = newX;
	b.y = newY;
}


void Block_Create(Cell board[ROW][COLUMN], std::vector<Block>& blocks, bool& block_down_flag, bool& game_end_flag) {
	int block{ block_r(gen)};
	int color{ color_r(gen) };
	int x, width, height;

	switch (block) {
	case 1:
	{
		x = 10;
		width = 1;
		height = 4;

		break;
	}
	case 2:
	{
		x = 8;
		width = 4;
		height = 1;

		break;
	}
	case 3:
	{
		x = 9;
		width = 2;
		height = 2;

		break;
	}
	}

	blocks.emplace_back(x, 0, width, height, color);

	Block& b = blocks[blocks.size() - 1];

	for (int i{ b.x }; i < b.x + b.width; i++) {
		for (int j{ b.y }; j < b.y + b.height; j++) {
			if (board[j][i].type) {
				game_end_flag = true;
				return;
			}
		}
	}

	block_down_flag = true;
}

void Block_Check(Cell board[ROW][COLUMN]) {
	for (int i{}; i <= ROW - 4; i++) {
		for (int j{}; j <= COLUMN; j++) {
			if (Block_4x4_Check(board, i, j)) {
				//삭제
				for (int y{ i }; y < i + 4; y++) {
					for (int x{ j }; x < j + 4; x++) {
						board[y][x].type = 0;
					}
				}

				for (int y{ i-1 }; y >= 0; y--) {
					for (int x{ j }; x < j + 4; x++) {
						board[y + 4][x] = board[y][x];
						board[y][x].type = 0;
					}
				}

				i = -1;
				break;
			}
		}
	}
}

bool Block_4x4_Check(Cell board[ROW][COLUMN], int sy, int sx) {
	for (int i{ sy }; i < sy + 4; i++) {
		for (int j{ sx }; j < sx + 4; j++) {
			if (i >= ROW || j >= COLUMN || not board[i][j].type) {
				return false;
			}
		}
	}
	return true;
}