#pragma once
#include <Windows.h>
#include <vector>
#include <random>
#include <cstdlib>
#include "GRID_PAINT.h"
#include "Circle.h"
#include "Shape_Paint.h"

std::random_device rd;  // �õ� ������
std::mt19937 gen(rd()); // Mersenne Twister ����
std::uniform_int_distribution<> dis(0, ROW-1); // 0 ~ 39

void Circle_Move(std::vector<Circle>& shapes, int board[ROW][COLUMN]);
void COORDINATE_SHIFT(Circle& c, int dx, int dy, int board[ROW][COLUMN]);

void Circle_Move(std::vector<Circle>& shapes, int board[ROW][COLUMN]) {
	for (int i{}; i < shapes.size(); i++) {
		Circle& s = shapes[i];
		
		switch(s.direction){ 
		case 1: //�¿� ������
			COORDINATE_SHIFT(s, s.direction_x, 0, board); //direction_x>0 ������/ else ����
			break;
		case -1: //���Ͽ�����
			COORDINATE_SHIFT(s, 0, s.direction_y, board); //direction_x>0 �Ʒ���/ else ����
			break;
		case 2: //�밢��
			COORDINATE_SHIFT(s, s.direction_x, s.direction_y, board);
			break;
		case 3:
			s.size_flag > 0 ? s.size++ : s.size--;
			if (s.size < -7 || s.size > 0)
				s.size_flag *= -1;
			break;
		}
	}
}

void COORDINATE_SHIFT(Circle& c, int dx, int dy, int board[ROW][COLUMN]) {

	int newX = c.x + dx;
	int newY = c.y + dy;

	if (newX > COLUMN - 1 || newX < 0) { //��� �˻�
		if (c.order > 0) {
			c.direction_y > 0 ? newY++ : newY--;
			if (newY > ROW - 1 || newY < 0) {
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

	if (newY > ROW - 1 || newY < 0) {
		if (c.order > 0) {
			c.direction_x > 0 ? newX++ : newX--;
			if (newX > COLUMN - 1 || newX < 0) {
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

	if (board[newY][newX] == 1) { //��ֹ� �˻�

		if (c.direction > 0) { //�¿�� ������ ��
			if (c.order > 0) { //���ΰ���
				if ((c.x == 0 and newX == 0) or (c.x == COLUMN-1 and newX == COLUMN-1)) { //��迡�� ��/�Ʒ��� �������� �� ��ֹ��� ���
					c.direction *= -1;
					newY -= c.direction_y * 2;
					c.direction_y *= -1;
					if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) {
						newY -= c.direction_y;
						c.direction *= -1;
					}
				}
				else { //���� ������ �� �� ��ֹ��� ���
					c.direction *= -1;
					newX = c.x;
					newY += c.direction_y;
					if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) { //������ �ٲ�µ� �űⰡ ��ֹ�
						newY -= c.direction_y * 2;
						c.direction_y *= -1;
						if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) { //�� �ٲ�µ� �ű⵵ ��ֹ��̸� �ݴ�������� ���ư�
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
					if (newY<0 || newY>ROW - 1 || board[newY][newX] == 1) {
						newY -= c.direction_y * 2;
						c.direction_y *= -1;
						if (board[newY][newX] == 1) {
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
					if (newX > COLUMN - 1 || newX < 0 || board[newY][newX] == 1) {
						newX -= c.direction_x;
						c.direction *= -1;
					}
				}
				else {
					c.direction *= -1;
					newY = c.y;
					newX += c.direction_x;
					if (newX > COLUMN - 1 || newX < 0 || board[newY][newX] == 1) {
						newX -= c.direction_x * 2;
						c.direction_x *= -1;
						if (newX > COLUMN - 1 || newX < 0 || board[newY][newX] == 1) {
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
				if (newX<0 || newX>COLUMN - 1 || board[newY][newX] == 1) {
					newX -= c.direction_x * 2;
					c.direction_x *= -1;
					if (board[newY][newX] == 1) {
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