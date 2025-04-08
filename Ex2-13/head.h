#pragma once
#include <windows.h>
#include <vector>
#include <time.h>
#include <string>
#include "GRID_PAINT.h"
#include "Player.h"

using namespace std;

void MovePlayer(int dx, int dy, int board[ROW][COLUMN], Player& player) {
	int newX = player.x + dx;
	int newY = player.y + dy;

	if (newX < 0 || newX > 19 || newY < 0 || newY > 19) {
		return;
	}

	if (dx == -1 and player.x == 1 and board[player.y][player.x - 1] ||
		dx == 1 and player.x == 18 and board[player.y][player.x + 1] ||
		dy == -1 and player.y == 1 and board[player.y - 1][player.x] ||
		dy == 1 and player.y == 18 and board[player.y + 1][player.x]) 
	{
		return;
	}

	player.x = newX;
	player.y = newY;

	int& current = board[player.y][player.x];
	int& target = board[player.y + dy][player.x + dx];

	if (current) {
		int temp = (current + (target - 'A')) % 'Z';
		if (temp < 'A')
			temp += 'A';
		else if (target)
			temp += 1;
		target = temp;
		current = 0;
	}
}

int Word_Check(const wstring& answer, const int board[], int start_p) {
	wstring word{};
	for (int i{ start_p }; i < COLUMN; i++) {
		if (not board[i])
			break;
		word += (char)board[i];
	}

	if (word == answer)
		return 1;
	return 0;
}