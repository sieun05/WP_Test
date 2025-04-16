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

void Block_Move(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3);
void Crash_Check(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3);
void Block_Check(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3);
void Block_Create(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3);

void Block_Move(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3, int& current_block, int& current_block_index) {
	switch (current_block) {
	case 1:
	{
		Block_1& b = b1[current_block_index];

		for (int i{ 1 }; i <= b.height; i++) {
			for (int j{ 1 }; j <= b.width; j++) {
				board[b.y][b.x].type = 0;
				board[b.y + i][b.x].type = 1;
				board[b.y + i][b.x].color = b.color;
				
			}
		}
		break;
	}
	case 2:
	{
		Block_2& b = b2[current_block_index];

		for (int i{ }; i < b.height; i++) {
			for (int j{ }; j < b.width; j++) {
				board[b.y][b.x].type = 0;
				board[b.y + i][b.x + j].type = 2;
				board[b.y + i][b.x + j].color = b.color;
			}
		}

		break;
	}
	case 3:
	{
		Block_3& b = b3[current_block_index];

		for (int i{ }; i <  b.height; i++) {
			for (int j{ }; j < b.width; j++) {
				board[b.y][b.x].type = 0;
				board[b.y+i][b.x + j].type = 3;
				board[b.y + i][b.x + j].color = b.color;
			}
		}
		break;
	}
	}
}

void Crash_Check(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3) {
	
}

void Block_Check(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3) {

}

void Block_Create(Cell board[ROW][COLUMN], std::vector<Block_1>& b1, std::vector<Block_2>& b2, std::vector<Block_3>& b3,int& current_block, int& current_block_index, int& block_down_flag, int& game_end_flag) {
	int block{ 1 };
	int color{ color_r(gen) };

	switch (block) {
	case 1:
	{
		b1.emplace_back(10, 0, color);
		Block_1& b = b1[b1.size() - 1];

		for (int i{ b.y }; i < b.y + b.height; i++) {
			for (int j{ b.x }; j < b.x + b.width; j++) {
				if (not board[i][j].type) {
					board[i][j].type = 1;
					board[i][j].color = b.color;
				}
				else {
					game_end_flag = 1;
					return;
				}
			}
		}

		current_block = block;
		current_block_index = b1.size() - 1;
		break;
	}
	case 2:
	{
		b2.emplace_back(10, 0, color);
		Block_2& b = b2[b2.size() - 1];

		for (int i{ b.y }; i < b.y + b.height; i++) {
			for (int j{ b.x }; j < b.x + b.width; j++) {
				if (not board[i][j].type) {
					board[i][j].type = 2;
					board[i][j].color = b.color;
				}
				else {
					game_end_flag = 1;
					return;
				}
			}
		}

		current_block = block;
		current_block_index = b2.size() - 1;
		break;
	}
	case 3:
	{
		b3.emplace_back(10, 0, color);
		Block_3& b = b3[b2.size() - 1];

		for (int i{ b.y }; i < b.y + b.height; i++) {
			for (int j{ b.x }; j < b.x + b.width; j++) {
				if (not board[i][j].type) {
					board[i][j].type = 3;
					board[i][j].color = b.color;
				}
				else {
					game_end_flag = 1;
					return;
				}
			}
		}

		current_block = block;
		current_block_index = b3.size() - 1;
		break;
	}
	}

	block_down_flag = 1;
}

