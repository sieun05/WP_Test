#pragma once
#include <Windows.h>
#include <vector>
#include <random>
#include <cstdlib>
#include "GRID_PAINT.h"
#include "Circle.h"
#include "Shape_Paint.h"

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, ROW-1); // 0 ~ 39

void Circle_Move(std::vector<Circle>& shapes, int board[ROW][COLUMN]);
void COORDINATE_SHIFT(Circle& c, int dx, int dy, int board[ROW][COLUMN]);

void Circle_Move(std::vector<Circle>& shapes, int board[ROW][COLUMN]) {
	for (int i{}; i < shapes.size(); i++) {
		Circle& s = shapes[i];
		
		switch(s.direction){ 
		case 1: //좌우 움직임
			COORDINATE_SHIFT(s, s.direction_x, 0, board); //direction_x>0 오른쪽/ else 왼쪽
			break;
		case -1: //상하움직임
			COORDINATE_SHIFT(s, 0, s.direction_y, board); //direction_x>0 아래쪽/ else 위쪽
			break;
		case 2: //대각선
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

	if (newX > COLUMN - 1 || newX < 0) { //경계 검사
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

	if (board[newY][newX] == 1) { //장애물 검사

		if (c.direction > 0) { //좌우로 움직일 때
			if (c.order > 0) { //주인공원
				if ((c.x == 0 and newX == 0) or (c.x == COLUMN-1 and newX == COLUMN-1)) { //경계에서 위/아래로 움직였을 때 장애물인 경우
					c.direction *= -1;
					newY -= c.direction_y * 2;
					c.direction_y *= -1;
					if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) {
						newY -= c.direction_y;
						c.direction *= -1;
					}
				}
				else { //경계로 가려고 할 때 장애물인 경우
					c.direction *= -1;
					newX = c.x;
					newY += c.direction_y;
					if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) { //방향을 바꿨는데 거기가 장애물
						newY -= c.direction_y * 2;
						c.direction_y *= -1;
						if (newY > ROW - 1 || newY < 0 || board[newY][newX] == 1) { //또 바꿨는데 거기도 장애물이면 반대방향으로 돌아감
							newY -= c.direction_y;
							c.direction *= -1;
							c.direction_x *= -1;
						}
					}
				}
			}
			else { //주인공원x
				if(abs(c.direction)==1){ //상하좌우
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
				else { //대각선
					c.direction_x > 0 ? newX -= 2 : newX += 2;
					c.direction_x *= -1;
					c.direction_y > 0 ? newY -= 2 : newY += 2;
					c.direction_y *= -1;
				}
			}
		}
		else { //상하
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
	} //장애물검사 end

	

	c.x = newX;
	c.y = newY;
}