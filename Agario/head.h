#pragma once
#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <vector>
#include <random>
#include <math.h>
#include <cstdlib>
#include <algorithm>
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
void Virus_Move(std::vector<Virus>& virus, const RECT& f_rect, const std::vector<Cell>& cell, std::vector<Feed>& feed);

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

		//int newx, newy
		float newx = c.x + nx * c.speed;
		float newy = c.y + ny * c.speed;

		float size{ c.size / 2 };
		RECT c_rect{ newx - size, newy - size, newx + size, newy + size };

		if (c_rect.left < 0) newx = size;
		if (c_rect.top < 0) newy = size;
		if (c_rect.right > MAP_WIDTH) newx -=  c.speed;
		if (c_rect.bottom > MAP_HEIGHT)  newy -= c.speed;

		if (newx < 0 or newy < 0) return;

		c.x = newx;
		c.y = newy;

		//경계 밖으로 나가면 세포의 좌표가 음수가 되어서 size, size로 좌표가 튐
		//경계 밖으로 나가지 않도록 하기
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

			float radius{ c.size / 2 };
			int dx{ c.x - f.x };
			int dy{ c.y - f.y };
			int length{ (int)sqrt(dx * dx + dy * dy) };

			if (length < radius) { //세포 - 먹이 충돌시
				c.size += (float)f.size / 15;
				feed_cnt++;
				feed.erase(feed.begin() + j);
				c.speed = (float)80 / c.size;
				if (c.speed < 3) c.speed = 3;
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

inline void Virus_Move(std::vector<Virus>& virus, const RECT& f_rect, const std::vector<Cell>& cell, std::vector<Feed>& feed)
{
	for (int i{}; i < virus.size(); i++) {
		bool near_flag{};

		Virus& v = virus.at(i);
		const Cell& c = cell.at(0);

		int dx{ c.x - v.x };
		int dy{ c.y - v.y };
		double length{ sqrt(dx * dx + dy * dy) };

		if (length < 250) {
			near_flag = true;
		}

		if (near_flag) {
			float dirX = (float)dx / (float)length;
			float dirY = (float)dy / (float)length;

			v.x += (int)(dirX * v.speed);
			v.y += (int)(dirY * v.speed);

			if (v.x - v.size < 0) v.x = v.size;
			else if (v.x + v.size > MAP_WIDTH) v.x = MAP_WIDTH - v.size;

			if (v.y - v.size < 0) v.y = v.size;
			else if (v.y + v.size > MAP_HEIGHT) v.y = MAP_HEIGHT - v.size;

			//주인공세포 - 바이러스 위치에 따라 바이러스가 주인공세포쪽으로 가도록 작성
		}
		else {

			for (int j{}; j < feed.size(); j++) {
				Feed& f = feed.at(j);

				int fdx{ v.x - f.x };
				int fdy{ v.y - f.y };
				double flength{ sqrt(fdx * fdx + fdy * fdy) };

				if (length < 300) {

					float dirX = (float)fdx / (float)length;
					float dirY = (float)fdy / (float)length;

					v.x += (int)(dirX * v.speed);
					v.y += (int)(dirY * v.speed);

					if (v.x - v.size < 0) v.x = v.size;
					else if (v.x + v.size > MAP_WIDTH) v.x = MAP_WIDTH - v.size;

					if (v.y - v.size < 0) v.y = v.size;
					else if (v.y + v.size > MAP_HEIGHT) v.y = MAP_HEIGHT - v.size;

					near_flag = true;
				}
			}

			if (not near_flag) {

				v.tick++;

				if (v.tick % 100 == 0) {
					v.angle = Virus::RandomAngle();
				}

				v.x += (int)(cosf(v.angle) * v.speed);
				v.y += (int)(sinf(v.angle) * v.speed);

				if (v.x - v.size < 0 or v.x + v.size > MAP_WIDTH) {
					v.angle = 3.14156265f - v.angle;
					if (v.x - v.size < 0) {
						v.x = v.size;
					}
					else if (v.x + v.size > MAP_WIDTH) {
						v.x = MAP_WIDTH - v.size;
					}
				}

				if (v.y - v.size < 0 or v.y + v.size > MAP_HEIGHT) {
					v.angle = -v.angle;
					if (v.y - v.size < 0) {
						v.y = v.size;
					}
					else if (v.y + v.size > MAP_HEIGHT) {
						v.y = MAP_HEIGHT - v.size;
					}
				}
			}
		}
	}
}
