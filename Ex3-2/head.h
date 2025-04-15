#pragma once
#include <Windows.h>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include "GRID_PAINT.h"
#include "Circle.h"
#include "Shape_Paint.h"
#include "Cell.h"

std::random_device rd;  // �õ� ������
std::mt19937 gen(rd()); // Mersenne Twister ����
std::uniform_int_distribution<> dis(0, ROW-1); // 0 ~ 39
std::uniform_int_distribution<> direction(-1, 3);
std::uniform_int_distribution<> size(-4, 0);
std::uniform_int_distribution<> color(0, 5);

void Circle_Move(std::vector<Circle>& shapes, Cell board[ROW][COLUMN], int& main_circles_num);
void Coordinate_Shift(Circle& c, int dx, int dy, Cell board[ROW][COLUMN]);
void Crash_Check(std::vector<Circle>& shapes, const Circle& cir, int index, Cell board[ROW][COLUMN], int& main_circles_num);
int FindIndexByOrder(const std::vector<Circle>& shapes, int target_order);
//void Change_Direction(std::vector<Circle>& shapes, int ddrt, int ddrt_x, int ddrt_y);

void Circle_Move(std::vector<Circle>& shapes, Cell board[ROW][COLUMN], int& main_circles_num) {
	for (int i{}; i < shapes.size(); i++) {
		Circle& s = shapes[i];
		
		switch(s.direction){ 
		case 1: //�¿� ������
			Coordinate_Shift(s, s.direction_x, 0, board); //direction_x>0 ������/ else ����
			break;
		case -1: //���Ͽ�����
			Coordinate_Shift(s, 0, s.direction_y, board); //direction_x>0 �Ʒ���/ else ����
			break;
		case 2: //�밢��
			Coordinate_Shift(s, s.direction_x, s.direction_y, board);
			break;
		case 3:
			s.size_flag > 0 ? s.size++ : s.size--;
			if (s.size < -7 || s.size > 0)
				s.size_flag *= -1;
			break;
		}	
	}

	for (int i{}; i < shapes.size(); i++) {
		Circle& s = shapes[i];

		Crash_Check(shapes, s, i, board, main_circles_num);
	}
}

void Coordinate_Shift(Circle& c, int dx, int dy, Cell board[ROW][COLUMN]) {

	int newX = c.x + dx;
	int newY = c.y + dy;

	if (newX < 0 || newX > COLUMN - 1) {	// ��� �˻� X
		if (c.order > 0) {
			c.direction_y > 0 ? newY++ : newY--;
			if (newY < 0 || newY > ROW - 1) {
				c.direction_y > 0 ? newY-=2 : newY+=2;
				c.direction_y *= -1;
			}
			newX = c.x;
		}
		else {
			c.direction_x > 0 ? newX-=2 : newX+=2;
		}
		c.direction_x *= -1;
	}

	if (newY < 0 || newY > ROW - 1) {	// ��� �˻� Y
		if (c.order > 0) {
			c.direction_x > 0 ? newX++ : newX--;
			if (newX < 0 || newX > COLUMN - 1) {
				c.direction_x > 0 ? newX-=2 : newX+=2;
				c.direction_x *= -1;
			}
			newY = c.y;
		}
		else {
			c.direction_y > 0 ? newY -= 2 : newY += 2;
		}
		c.direction_y *= -1;
	}

	if (board[newY][newX].type == 1) { //��ֹ� �˻�

		if (c.direction > 0) { //�¿�� ������ ��
			if (c.order > 0) { //���ΰ���
				if ((c.x == 0 and newX == 0) or (c.x == COLUMN-1 and newX == COLUMN-1)) { //��迡�� ��/�Ʒ��� �������� �� ��ֹ��� ���
					c.direction *= -1;
					newY -= c.direction_y * 2;
					c.direction_y *= -1;
					if (newY < 0 || newY > ROW - 1 || board[newY][newX].type == 1) {
						newY -= c.direction_y;
						c.direction *= -1;
					}
				}
				else { //���� ������ �� �� ��ֹ��� ���
					c.direction *= -1;
					newX = c.x;
					newY += c.direction_y;
					if (newY < 0 || newY > ROW - 1 || board[newY][newX].type == 1) { //������ �ٲ�µ� �űⰡ ��ֹ�
						newY -= c.direction_y * 2;
						c.direction_y *= -1;
						if (newY < 0 || newY > ROW - 1 || board[newY][newX].type == 1) { //�� �ٲ�µ� �ű⵵ ��ֹ��̸� �ݴ�������� ���ư�
							newY -= c.direction_y;
							c.direction *= -1;
							c.direction_x *= -1;
						}
					}
				}
			}
			else { //���ΰ���x
				if(abs(c.direction)==1){ //�����¿�
					newX = c.x;
					newY += c.direction_y;
					if (newY<0 || newY>ROW - 1 || board[newY][newX].type == 1) {
						newY -= c.direction_y * 2;
						c.direction_y *= -1;
						if (board[newY][newX].type == 1) {
							newX -= c.direction_x;
							c.direction_x *= -1;
						}
					}
					c.direction *= -1;
				}
				else { //�밢��
					c.direction_x > 0 ? newX -= 2 : newX += 2;
					c.direction_x *= -1;
					c.direction_y > 0 ? newY -= 2 : newY += 2;
					c.direction_y *= -1;
				}
			}
		}
		else { //����
			if (c.order > 0) {
				if ((c.y==0 and newY == 0) or (c.y==ROW-1 and newY == ROW-1)) {
					c.direction *= -1;
					newX -= c.direction_x * 2;
					c.direction_x *= -1;
					if (newX > COLUMN - 1 || newX < 0 || board[newY][newX].type == 1) {
						newX -= c.direction_x;
						c.direction *= -1;
					}
				}
				else {
					c.direction *= -1;
					newY = c.y;
					newX += c.direction_x;
					if (newX > COLUMN - 1 || newX < 0 || board[newY][newX].type == 1) {
						newX -= c.direction_x * 2;
						c.direction_x *= -1;
						if (newX > COLUMN - 1 || newX < 0 || board[newY][newX].type == 1) {
							newX -= c.direction_x;
							c.direction *= -1;
							c.direction_y *= -1;
						}
					}
				}
			}
			else {
				newY = c.y;
				newX += c.direction_x;
				if (newX<0 || newX>COLUMN - 1 || board[newY][newX].type == 1) {
					newX -= c.direction_x * 2;
					c.direction_x *= -1;
					if (board[newY][newX].type == 1) {
						newX -= c.direction_x;
						c.direction_x *= -1;
					}
				}

				c.direction *= -1;
			}
		}
	} //��ֹ��˻� end

	c.x = newX;
	c.y = newY;
}

inline void Crash_Check(std::vector<Circle>& shapes, const Circle& cir, int index, Cell board[ROW][COLUMN], int& main_circles_num)
{
	if (cir.order==1) { //���ΰ��� - �׳ɿ� �浹
		for (int i{ 1 }; i < shapes.size(); i++) {
			Circle& c = shapes[i];
			if (c.order > 0) continue; //���ΰ���(������¾ֵ�) ����

			//��ǥ����
			if (c.x == cir.x and c.y == cir.y) {
				int idx = FindIndexByOrder(shapes, main_circles_num);
				if (idx == -1 || idx == i) return;
				
				const Circle& end_cir = shapes[idx];
				int color_temp{ c.color };
				c = end_cir;
				c.order++;
				c.color = color_temp;

				if (end_cir.direction > 0) {
					c.x -= end_cir.direction_x;
					if (c.x < 0 || c.x > COLUMN - 1) {
						c.x += end_cir.direction_x;
						c.y -= end_cir.direction_y;
						c.direction_x *= -1;
					}
					else {
						c.y = end_cir.y;
					}
				}
				else {
					c.y -= end_cir.direction_y;
					if (c.y < 0 || c.y > ROW - 1) {
						c.y += end_cir.direction_y;
						c.x -= end_cir.direction_x;
						c.direction_y *= -1;
					}
					else {
						c.x = end_cir.x;
					}
				}
				main_circles_num++;
			}	
		}

		if (cir.x >= 0 && cir.x < COLUMN && cir.y >= 0 && cir.y < ROW) {
			if (board[cir.y][cir.x].type == 2) { //���̶� �浹
				board[cir.y][cir.x].type = 0;
				int d{ direction(gen) };
				int s{ size(gen) };
				shapes.emplace_back(cir.x, cir.y, d, s, 0, board[cir.y][cir.x].color);
			}
		}
	}
	else if(cir.order==0){ //�Ϲݿ����� �ε�����
		for (int i{ 1 }; i < shapes.size(); i++) {
			Circle& c = shapes[i];
			if (c.order > 0 or index==i) continue;

			if (c.x == cir.x and c.y == cir.y) {

				int color_temp{ c.color };
				c = cir;
				c.color = color_temp;

				switch (cir.direction) { //�׷��� ������������
				case 1:
					c.x -= cir.direction_x;
					if (c.x < 0 || c.x > COLUMN - 1) {
						c.x += cir.direction_x;
						c.y -= cir.direction_y;
						c.direction_x *= -1;
					}
					else {
						c.y = cir.y;
					}
					break;
				case -1:
					c.y -= cir.direction_y;
					if (c.y < 0 || c.y > ROW - 1) {
						c.y += cir.direction_y;
						c.x -= cir.direction_x;
						c.direction_y *= -1;
					}
					else {
						c.x = cir.x;
					}
					break;
				case 0:
				case 3:
					shapes.erase(shapes.begin() + i);
					break;
				case 2:
					c.x -= cir.direction_x;
					if (c.x < 0 || c.x > COLUMN - 1) {
						c.x += cir.direction_x;
						c.y -= cir.direction_y;
						c.direction_x *= -1;
					}
					else {
						c.y = cir.y;
					}

					c.y -= cir.direction_y;
					if (c.y < 0 || c.y > ROW - 1) {
						c.y += cir.direction_y;
						c.x -= cir.direction_x;
						c.direction_y *= -1;
					}
					else {
						c.x = cir.x;
					}
					break;
				}
			}
		}
	}
}

inline int FindIndexByOrder(const std::vector<Circle>& shapes, int target_order)
{
	auto it = std::find_if(shapes.begin(), shapes.end(), [&](const Circle& c) {
		return c.order == target_order;
	});

	if (it != shapes.end())
		return std::distance(shapes.begin(), it);
	else
		return -1;
}

//inline void Change_Direction(std::vector<Circle>& shapes, int ddrt, int ddrt_x, int ddrt_y) //�� �𸣰ٴ�...
//{
//	for (int i{}; i < shapes.size(); i++) {
//		Circle& c = shapes[i];
//		if (c.order > 0) {
//			c.di
//		}
//	}
//}
