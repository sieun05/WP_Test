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
#include "Item.h"

#define MAP_WIDTH 1280
#define MAP_HEIGHT 670

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> r_width(0, MAP_WIDTH);
uniform_int_distribution<> r_height(0, MAP_HEIGHT);
uniform_int_distribution<> r_color(0, 5);
uniform_int_distribution<> r_flag(0, 3);
uniform_int_distribution<> r_virus_devide(1, 100);

void Cell_Move(std::vector<Cell>& cell, int mx, int my, const RECT& clientRect, const int& camera_mode);
void Feed_Crash(std::vector<Cell>& cell, std::vector<Feed>& feed, int& feed_cnt);
void Feed_Crash(std::vector<Virus>& virus, std::vector<Feed>& feed);
void Virus_Move(std::vector<Virus>& virus, const RECT& f_rect, const std::vector<Cell>& cell, std::vector<Feed>& feed);
void Virus_Crash(std::vector<Cell>& cell, std::vector<Virus>& virus, bool& game_end_flag);
void Item_Move(Item& item);
void Item_Crash(std::vector<Cell>& cell, Item& item, bool& item_flag);

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
		if (c_rect.right > MAP_WIDTH - 5) newx -=  c.speed;
		if (c_rect.bottom > MAP_HEIGHT)  newy -= c.speed;

		if (newx < 0 or newy < 0) return;

		c.x = newx;
		c.y = newy;

		c.delay++;
		if (c.DESC_flag and c.delay > 80 * 20) {
			Cell& c0 = cell.at(0);

			int ddx{ c0.x - c.x };
			int ddy{ c0.y - c.y };
			double dlength{ sqrt(ddx * ddx + ddy * ddy) };

			if (dlength < c0.size / 2) {
				c0.size += c.size * 0.8;
				cell.erase(cell.begin() + i);
				i--;
			}

			float dirX = (float)ddx / (float)dlength;
			float dirY = (float)ddy / (float)dlength;

			c.x += (int)(dirX * c.speed);
			c.y += (int)(dirY * c.speed);
		}

	}
}
inline void Feed_Crash(std::vector<Cell>& cell, std::vector<Feed>& feed, int& feed_cnt)
{
	for (int i{}; i < cell.size(); i++) {
		Cell& c = cell.at(i);

		for (int j{}; j < feed.size(); j++) {
			Feed& f = feed.at(j);
			f.time++;

			if (f.time > 80 * 40) { //생긴지 20초 경과 > 없어짐
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
				c.speed = (float)100 / c.size;
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
				v.go_flag = false;
				v.size += f.size / 7;
				feed.erase(feed.begin() + j);
				v.speed = (float)60 / v.size;
				if (v.speed < 2.5) v.speed = 2.5;

				if (not v.DESC_flag and v.DESC_cnt < 2 and v.size > 30) { //바이러스의 크기가 일정 수준 이상이 되었을 때,
					v.size /= 2;
					v.DESC_cnt++;
					virus.emplace_back(v.x + v.size * 2, v.y + v.size * 2, v.size, v.color, true, i, v. DESC_cnt);
				}
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

		if (not v.DESC_flag) {
			const Cell& c = cell.at(0);

			int dx{ c.x - v.x };
			int dy{ c.y - v.y };
			double length{ sqrt(dx * dx + dy * dy) };

			if (length < c.size/2 * 1.5 or length < 150) {
				near_flag = true;
			}

			if (near_flag) {
				float dirX = (float)dx / (float)length;
				float dirY = (float)dy / (float)length;

				v.x += (int)(dirX * v.speed);
				v.y += (int)(dirY * v.speed);
			}
			else {
				if (not v.go_flag) {
					v.go_flag = true;

					int near_feed_index{};
					double near_length{ 99999 };

					for (int j{}; j < feed.size(); j++) {
						Feed& f = feed.at(j);

						int fdx{ f.x - v.x };
						int fdy{ f.y - v.y };
						double flength{ sqrt(fdx * fdx + fdy * fdy) };

						if (flength < near_length) {
							near_feed_index = j;
							near_length = flength;
						}
					}
					v.go_feed_index = near_feed_index;
				}

				if (v.go_feed_index >= feed.size()) {
					v.go_flag = false;
					continue;
				}

				int fdx{ feed.at(v.go_feed_index).x - v.x };
				int fdy{ feed.at(v.go_feed_index).y - v.y };
				double flength{ sqrt(fdx * fdx + fdy * fdy) };

				// 가까워졌다면 다시 feed를 찾도록 플래그 초기화
				if (flength < v.speed * 1.5) {
					v.go_flag = false;
					continue;
				}

				float dirX = (float)fdx / (float)flength;
				float dirY = (float)fdy / (float)flength;

				v.x += (int)(dirX * v.speed);
				v.y += (int)(dirY * v.speed);
			}
		}
		else {
			if (v.ASC_num == i) continue;
			if (v.ASC_num >= virus.size()) continue;

			v.tick++;

			Virus& c = virus.at(v.ASC_num);

			int dx{ c.x - v.x };
			int dy{ c.y - v.y };
			double length{ sqrt(dx * dx + dy * dy) };

			if (v.tick > 80 * 10) {
				dx={ c.x - v.x };
				dy={ c.y - v.y };
				length={ sqrt(dx * dx + dy * dy) };

				if (length < c.size / 2) {
					c.size += v.size * 0.7;
					virus.erase(virus.begin() + i);
					//c.DESC_cnt--;
					i--;
				}

				float dirX = (float)dx / (float)length;
				float dirY = (float)dy / (float)length;

				v.x += (int)(dirX * v.speed);
				v.y += (int)(dirY * v.speed);
			}
			else {
				if (v.DESC_order == 1) {
					v.x = c.x + (int)(1 * c.size);
					v.y = c.y + (int)(1 * c.size);
				}
				else {
					v.x = c.x + (int)(1 * c.size);
					v.y = c.y + (int)(-1 * c.size);
				}
			}
		}
	}
}
void Virus_Crash(std::vector<Cell>& cell, std::vector<Virus>& virus, bool& game_end_flag) {
	for (int i{}; i < cell.size(); i++) {
		Cell& c = cell.at(i);

		for (int j{}; j < virus.size(); j++) {
			Virus& v = virus.at(j);

			int dx{ c.x - v.x };
			int dy{ c.y - v.y };
			double length{ sqrt(dx * dx + dy * dy) };

			if (length < c.size / 2) {
				game_end_flag = true;
				return;
			}
		}
	}
}

inline void Item_Move(Item& i)
{
	i.x += (int)(i.dirX * i.speed);
	i.y += (int)(i.dirY * i.speed);

	// x 경계 충돌 처리
	if (i.x - i.size < 0) {
		i.x = i.size;
		i.dirX *= -1;
	}
	else if (i.x + v.size > MAP_WIDTH) {
		i.x = MAP_WIDTH - i.size;
		i.dirX *= -1;
	}

	// y 경계 충돌 처리
	if (i.y - i.size < 0) {
		i.y = i.size;
		i.dirY *= -1;
	}
	else if (i.y + i.size > MAP_HEIGHT) {
		i.y = MAP_HEIGHT - i.size;
		i.dirY *= -1;
	}

	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> turn_chance(0.0f, 1.0f);
	if (turn_chance(rng) < 0.01f) { // 1% 확률로 방향 변경
		i.setRandomDirection();
	}
}
