#pragma once
#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <random>
#include "GRID_PAINT.h"
#include "Cell.h"
#include "resource.h"

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> row(0, ROW - 1); // 0 ~ 39
std::uniform_int_distribution<> col(0, COL - 1); // 0 ~ 39

void Score_Check(Cell board[ROW][COL], int& Score) {
	int scr{};

	for (int k{ 3 }; k < 8; k++) {
		int cnt{};

		for (int i{}; i < ROW; i++) {
			for (int j{}; j < COL; j++) {
				if (board[i][j].type == k and board[i][j].open_flag)
					cnt++;
			}
		}

		if (cnt == 4)
			scr++;
	}

	Score = scr;
}

void Game_End_Check(Cell board[ROW][COL], bool& game_end_flag, bool& game_check) {
	bool all_pie_open{ true };

	for (int i{}; i < ROW; i++) {
		for (int j{}; j < COL; j++) {
			if (board[i][j].type >= 3 and board[i][j].type <= 7) {
				if (not board[i][j].open_flag)
					all_pie_open = false;
			}
		}
	}

	if (all_pie_open) {
		game_end_flag = true;
		game_check = true;
	}
}