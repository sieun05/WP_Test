#pragma once
#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <vector>
#include <random>
#include <math.h>
#include <algorithm>
#include "GRID_PAINT.h"
#include "Obstacle.h"

#define PI 3.141592

using namespace std;

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, 1); // 0 ~ 39
std::uniform_int_distribution<> rcol(1, 4); // 0 ~ 39

void Obstacle_Move(Obstacle obstacle[], int& obstacle_move, const float& obstacle_speed, int flag_y);
void Obstacle_Move(std::vector<Obstacle2>& obstacle, std::vector<Obstacle>& R2_obstacle_pos,
	POINT& R2_center, int& obstacle_move, const float& obstacle_speed);
void Player_Move(
	const RECT& f_rect, const RECT player_field[2], int board[R2_ROW][R2_COL], const int& round,
	POINT& p, int dx, int dy, bool& game_end_flag, const std::vector<POINT>& item);
void Player_Crash(const int& round, const POINT& p, std::vector<POINT>& item, Obstacle obstacle[],
	std::vector<Obstacle> R2_obstacle_pos, bool& reset_flag, const bool& keyS_flag);
bool IsInsideMap(POINT p, POINT p_end, const RECT& f_rect, const RECT player_field[2]);
bool IsRectInside(const RECT& outer, const RECT& inner);
float LengthPts(int x1, int y1, int x2, int y2);
bool InCircle(int x1, int y1, int x2, int y2, int r);

void Obstacle_Move(Obstacle obstacle[], int& obstacle_move, const float& obstacle_speed, int flag_y) {
	for (int i{}; i < 12; i++) {
		if (obstacle_move) {
			(i % 2 == 0) ? obstacle[i].y -= obstacle_speed : obstacle[i].y += obstacle_speed;
			if (obstacle[0].y <= flag_y + 6) {
				obstacle[0].y += obstacle_speed;
				obstacle_move = 0;
				break;
			}
		}
		else {
			(i % 2 != 0) ? obstacle[i].y -= obstacle_speed : obstacle[i].y += obstacle_speed;
			if (obstacle[0].y >= flag_y + 174) {
				obstacle[0].y -= obstacle_speed;
				obstacle_move = 1;
				break;
			}
		}
	}
}

void Obstacle_Move(std::vector<Obstacle2>& obstacle, std::vector<Obstacle>& R2_obstacle_pos, 
	POINT& R2_center, int& obstacle_move, const float& obstacle_speed) {
	R2_obstacle_pos.clear();

	for (auto& ob : obstacle) {
		ob.angle += (obstacle_move ? obstacle_speed : -obstacle_speed);

		POINT pt;
		pt.x = R2_center.x + (int)(ob.radius * cos(ob.angle));
		pt.y = R2_center.y + (int)(ob.radius * sin(ob.angle));

		R2_obstacle_pos.emplace_back(pt.x, pt.y, ob.color);
	}
}

void Player_Move(
	const RECT& f_rect, const RECT player_field[2], int board[R2_ROW][R2_COL], const int& round, 
	POINT& p, int dx, int dy, bool& game_end_flag, const std::vector<POINT>& item) {

	POINT newP{ p.x + dx * 3,  p.y + dy * 3 };
	POINT newEndP{ newP.x + 20, newP.y + 20 };

	RECT rectp{ newP.x ,newP.y, newEndP.x, newEndP.y };

	if (not IsInsideMap(newP, newEndP, f_rect, player_field))
		return;

	if (IsRectInside(player_field[1], rectp) && item.empty()) {
		game_end_flag = true;
	}

	if (round == 2) {
		const int tile_size{ 30 };
		int col{ (newP.x - f_rect.left) / tile_size };
		int row{ (newP.y - f_rect.top) / tile_size };
		int col2{ (newEndP.x - f_rect.left) / tile_size };
		int row2{ (newEndP.y - f_rect.top) / tile_size };

		if ((row < 0 or row >= R2_ROW or col < 0 or col >= R2_COL) or (row2 < 0 or row2 >= R2_ROW or col2 < 0 or col2 >= R2_COL)) {
			if (IsInsideMap(newP, newEndP, f_rect, player_field)) {
				p = newP;
			}
			else {
				return;
			}
		}

		if (board[row][col] == 1 and board[row2][col2] == 1 and IsInsideMap(newP, newEndP, f_rect, player_field)) {
			p = newP;
		}
	}
	else {
		p = newP;
	}

}

void Player_Crash(const int& round, const POINT& p, std::vector<POINT>& item, Obstacle obstacle[], 
	std::vector<Obstacle> R2_obstacle_pos, bool& reset_flag, const bool& keyS_flag) {

	RECT rectp{ p.x, p.y, p.x+20, p.y+20};

	for (int i{}; i < item.size(); i++) {
		if (PtInRect(&rectp, item.at(i))) {
			item.erase(item.begin() + i);
		}
	}

	if(round==1){
		for (int i{}; i < 12; i++) {
			if (keyS_flag) break;

			POINT obp{ obstacle[i].x, obstacle[i].y };

			if (PtInRect(&rectp, obp)) {
				reset_flag = true;
			}
		}
	}
	else if (round == 2) {
		for (int i{}; i < R2_obstacle_pos.size(); i++) {
			if (keyS_flag) break;

			POINT obp{ R2_obstacle_pos[i].x, R2_obstacle_pos[i].y };

			if (PtInRect(&rectp, obp)) {
				reset_flag = true;
			}
		}
	}
}

bool IsInsideMap(POINT p, POINT p_end, const RECT& f_rect, const RECT player_field[2]) {
	return
		(
			PtInRect(&f_rect, p) ||
			PtInRect(&player_field[0], p) ||
			PtInRect(&player_field[1], p)) and
		(
			PtInRect(&f_rect, p_end) ||
			PtInRect(&player_field[0], p_end) ||
			PtInRect(&player_field[1], p_end));
}

bool IsRectInside(const RECT& outer, const RECT& inner) {
	return (inner.left >= outer.left &&
		inner.top >= outer.top &&
		inner.right <= outer.right &&
		inner.bottom <= outer.bottom);
}

float LengthPts(int x1, int y1, int x2, int y2) {
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

bool InCircle(int x1, int y1, int x2, int y2, int r) {
	if (LengthPts(x1, y1, x2, y2) < r)
		return true;
	else
		return false;
}