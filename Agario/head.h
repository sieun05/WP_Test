#pragma once
#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <vector>
#include <random>
#include <math.h>
#include <cstdlib>
#include "resource.h"
#include "Cell.h"
#include "GRID_PAINT.h"
#include "Feed.h"
#include "Virus.h"

#define MAP_WIDTH 1280
#define MAP_HEIGHT 720

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> r_width(0, MAP_WIDTH);
uniform_int_distribution<> r_height(0, MAP_HEIGHT);
uniform_int_distribution<> r_color(0, 5);
uniform_int_distribution<> r_flag(0, 3);
uniform_int_distribution<> r_virus_direct(0, 20);

void Cell_Move(std::vector<Cell>& cell, int mx, int my, const RECT& clientRect, const int& camera_mode);
void Feed_Crash(std::vector<Cell>& cell, std::vector<Feed>& feed, int& feed_cnt);
void Feed_Crash(std::vector<Virus>& virus, std::vector<Feed>& feed);
void Virus_Move(std::vector<Virus>& virus, const RECT& f_rect, const std::vector<Cell>& cell);

void Cell_Move(std::vector<Cell>& cell, int mx, int my, const RECT& clientRect, const int& camera_mode) {

	POINT mp{ mx, my };
	if (not PtInRect(&clientRect, mp)) return;

	for (int i{}; i < cell.size(); i++) {
		Cell& c = cell.at(i);

		int centerX{ (clientRect.left + clientRect.right) / 2 };
		int centerY{ (clientRect.top + clientRect.bottom) / 2 };

		float dx{};
		float dy{};

		if (camera_mode == 1) {
			dx = (float)(mx - centerX);
			dy = (float)(my - centerY);
		}
		else {
			dx = (float)(mx - c.x);
			dy = (float)(my - c.y);
		}
		float length = sqrtf(dx * dx + dy * dy);

		//단위벡터
		float nx = dx / length;
		float ny = dy / length;

		//int newx, newy;

		float newx = c.x + nx * c.speed;
		float newy = c.y + ny * c.speed;

		int size{ c.size / 2 };
		RECT c_rect{ newx - size, newy - size, newx + size, newy + size };

		if (c_rect.left < 0) newx = size;
		if (c_rect.top < 0) newy = size;
		if (c_rect.right > MAP_WIDTH) newx = MAP_WIDTH - size;
		if (c_rect.bottom > MAP_HEIGHT)  newy = MAP_HEIGHT - size;

		c.x = newx;
		c.y = newy;
	}
}

inline void Feed_Crash(std::vector<Cell>& cell, std::vector<Feed>& feed, int& feed_cnt)
{
	for (int i{}; i < cell.size(); i++) {
		Cell& c = cell.at(i);

		for (int j{}; j < feed.size(); j++) {
			Feed& f = feed.at(j);
			f.time++;

			if (f.time > 80 * 20) { //생긴지 20초 경과 > 없어짐
				feed.erase(feed.begin() + j);
				j--;
				continue;
			}

			if (abs(c.x - f.x) > c.size or abs(c.y - f.y) > c.size + 10) //세포와 가깝지 않으면 연산x
				continue;

			int radius{ c.size / 2 };
			int dx{ c.x - f.x };
			int dy{ c.y - f.y };
			int length{ (int)sqrt(dx * dx + dy * dy) };

			if (length < radius) { //세포 - 먹이 충돌시
				c.size += f.size / 10;
				feed_cnt++;
				feed.erase(feed.begin() + j);
				c.speed = (float)60 / c.size;
				j--;
			}
		}
	}
}

inline void Feed_Crash(std::vector<Virus>& virus, std::vector<Feed>& feed)
{
	for (int i{}; i < virus.size(); i++) {
		Virus& v = virus.at(i);

		for (int j{}; j < feed.size(); j++) {
			Feed& f = feed.at(j);

			if (abs(v.x - f.x) > v.size or abs(v.y - f.y) > v.size + 10) //바이러스와와 가깝지 않으면 연산x
				continue;

			int radius{ v.size / 2 };
			int dx{ v.x - f.x };
			int dy{ v.y - f.y };
			int length{ (int)sqrt(dx * dx + dy * dy) };

			if (length < radius) { //세포 - 먹이 충돌시
				v.size += f.size / 10;
				feed.erase(feed.begin() + j);
				v.speed = (float)60 / v.size;
				j--;
			}
		}
	}
}

inline void Virus_Move(std::vector<Virus>& virus, const RECT& f_rect, const std::vector<Cell>& cell)
{
	for (int i{}; i < virus.size(); i++) {
		bool near_flag{};

		Virus& v = virus.at(i);
		const Cell& c = cell.at(0);

		int dx{ c.x - v.x };
		int dy{ c.y - v.y };
		int length{ sqrt(dx * dx + dy * dy) };

		if (length < 250) {
			near_flag = true;
		}

		if (near_flag) {
			int 

				//주인공세포 - 바이러스 위치에 따라 바이러스가 주인공세포쪽으로 가도록 작성
		}
		else {
			bool r_direct_x{ r_virus_direct(gen) == 20 };
			bool r_direct_y{ r_virus_direct(gen) == 20 };

			r_direct_x ? v.direction_x *= -1 : v.direction_x;
			r_direct_y ? v.direction_y *= -1 : v.direction_y;

			//위치 이동, 경계에서 튕기기
		}
	}
}
