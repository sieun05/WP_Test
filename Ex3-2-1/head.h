// head.h
#pragma once
#include <Windows.h>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "GRID_PAINT.h"
#include "Circle.h"
#include "Cell.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, ROW - 1);
std::uniform_int_distribution<> direction(-1, 3);
std::uniform_int_distribution<> size(-4, 0);
std::uniform_int_distribution<> color(0, 5);

bool has_tail(const Circle& head, const std::vector<Circle>& shapes, int head_index);

void Coordinate_Shift(Circle& c, Cell board[ROW][COLUMN]) {
	int newX = c.x;
	int newY = c.y;

	if (c.direction == 1) {
		newX += c.direction_x;
	}
	else if (c.direction == -1) {
		newY += c.direction_y;
	}

	if (newX < 0 || newX >= COLUMN || newY < 0 || newY >= ROW) {
		if (c.direction == 1) {
			newX = c.x;
			newY += c.direction_y;
		}
		else if (c.direction == -1) {
			newY = c.y;
			newX += c.direction_x;
		}
		c.direction_x *= -1;
		c.direction_y *= -1;
	}
	else if (board[newY][newX].type == 1) {
		c.direction *= -1;
		if (c.direction == 1) {
			newX = c.x;
			newY += c.direction_y;
		}
		else if (c.direction == -1) {
			newY = c.y;
			newX += c.direction_x;
		}
		if (newX < 0 || newX >= COLUMN || newY < 0 || newY >= ROW || board[newY][newX].type == 1) {
			newX = c.x;
			newY = c.y;
		}
	}

	c.x = newX;
	c.y = newY;
}

void Circle_Move(std::vector<Circle>& shapes, Cell board[ROW][COLUMN]) {
	for (auto& c : shapes) {
		c.prev_x = c.x;
		c.prev_y = c.y;
	}

	for (auto& c : shapes) {
		if (c.parent_index == -1) {
			Coordinate_Shift(c, board);
		}
	}

	for (auto& c : shapes) {
		if (c.parent_index != -1) {
			Circle& parent = shapes[c.parent_index];
			c.x = parent.prev_x;
			c.y = parent.prev_y;
		}
	}
}

void AddTail(std::vector<Circle>& shapes) {
	if (shapes.empty()) return;
	Circle& last = shapes.back();
	Circle newTail(last.prev_x, last.prev_y, last.direction, last.size, last.color);
	newTail.parent_index = (int)shapes.size() - 1;
	shapes.push_back(newTail);
}

void Crash_Check(std::vector<Circle>& shapes) {
	for (int i = 0; i < shapes.size(); ++i) {
		for (int j = i + 1; j < shapes.size(); ++j) {
			if (shapes[i].x == shapes[j].x && shapes[i].y == shapes[j].y) {
				Circle& a = shapes[i];
				Circle& b = shapes[j];

				bool a_main = a.parent_index == -1;
				bool b_main = b.parent_index == -1;

				bool a_has_tail = has_tail(a, shapes, i);
				bool b_has_tail = has_tail(b, shapes, j);

				// 주인공 원 - 일반 원 충돌
				if (a_main && a_has_tail && b_main && !b_has_tail) {
					b.parent_index = i;
				}
				else if (b_main && b_has_tail && a_main && !a_has_tail) {
					a.parent_index = j;
				}

				// 일반 원 - 일반 원 충돌
				else if (a_main && !a_has_tail && b_main && !b_has_tail) {
					b.parent_index = i;
				}
			}
		}
	}
}

bool has_tail(const Circle& head, const std::vector<Circle>& shapes, int head_index) {
	for (int i = 0; i < shapes.size(); ++i) {
		if (shapes[i].parent_index == head_index) return true;
	}
	return false;
}


//inline int FindIndexByOrder(const std::vector<Circle>& shapes, int target_order)
//{
//	auto it = std::find_if(shapes.begin(), shapes.end(), [&](const Circle& c) {
//		return c.order == target_order;
//		});
//
//	if (it != shapes.end())
//		return std::distance(shapes.begin(), it);
//	else
//		return -1;
//}
//
//inline void T_Down(std::vector<Circle>& shapes, const int& main_circles_num)
//{
//	for (int i{}; i < shapes.size(); i++) {
//		Circle& c = shapes[i];
//		if (c.order > 0) {
//			if (c.order == 1) {
//				c.order = main_circles_num;
//				continue;
//			}
//			c.order--;
//		}
//	}
//}
//
//inline void Circle_Jump(const std::vector<Circle>& shapes, Cell board[ROW][COLUMN], int& jump_count, bool& jump_flag, const int& main_circles_num)
//{
//	static int j_x[3]{}, j_y[3]{};
//	if (not jump_flag) {
//		jump_flag = true;
//
//		int idx = FindIndexByOrder(shapes, 2);
//
//		j_y[0] = shapes[idx].x;
//		j_y[0] = shapes[idx].y;
//
//		j_x[1] = shapes[0].x;
//		j_y[1] = shapes[0].y;
//
//		if (shapes[0].direction == 1) {
//			j_x[2] = shapes[0].x + shapes[0].direction;
//			j_y[2] = shapes[0].y;
//		}
//		else {
//			j_x[2] = shapes[0].x;
//			j_y[2] = shapes[0].y + shapes[0].direction;
//		}
//
//		board[j_y[0]][j_x[0]].type = 7;
//		board[j_y[1]][j_x[1]].type = 7;
//		board[j_y[2]][j_x[2]].type = 7;
//	}
//	else {
//		jump_count--;
//		if (jump_count == 0) {
//			board[j_y[0]][j_x[0]].type = 0;
//			board[j_y[1]][j_x[1]].type = 0;
//			board[j_y[2]][j_x[2]].type = 0;
//			jump_flag = false;
//		}
//	}
//}

//inline void Change_Direction(std::vector<Circle>& shapes, int ddrt, int ddrt_x, int ddrt_y) //오 모르겟다...
//{
//	for (int i{}; i < shapes.size(); i++) {
//		Circle& c = shapes[i];
//		if (c.order > 0) {
//			c.di
//		}
//	}
//}
