#pragma once
//Ex3-5
#include <Windows.h>
#include <vector>
#include <random>
#include "GRID_PAINT.h"
#include "Car.h"

using namespace std;

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, 1); // 0 ~ 39

void Car_Move(Car car[], int traffic_light, const RECT& f_rect, int move_length_s, bool traffic_stop_flag, POINT& p, int& player_move, bool& player_move_flag) {
	int centerX{ (f_rect.left + f_rect.right) / 2 };
	int centerY{ (f_rect.top + f_rect.bottom) / 2 };
	int width{ f_rect.right - f_rect.left };
	int height{ f_rect.bottom - f_rect.top };
	int length_w{ (centerX - 150 - 100) - f_rect.left };
	int length_h{ (centerY - 150 - 100) - f_rect.top };


	int prev_positions[8]{};
	for (int i{}; i < 8; i++) {
		prev_positions[i] = car[i].position;
	}

	for (int i{}; i < 8; i++) {

		int cur_traffic = (traffic_light==3) ? 1: traffic_light;
		if (traffic_stop_flag)
			cur_traffic = 0;
		else
			if (i < 4) cur_traffic = 2 - traffic_light;

		Car& c = car[i];
		int c2_pos = (i % 2 == 0) ? prev_positions[i + 1] : prev_positions[i - 1];
		int move_length = move_length_s + c.speed;

		switch (cur_traffic) {
		case 0:
		{
			//차 이동
			if (c.position > move_length) { //이미 선을 넘어버렸을 때
				c.position += move_length;
				if (c2_pos > c.position and c2_pos - c.position <= 140)
					c.position -= move_length;
			}
			else if ((c.position < 0 and c2_pos > c.position and c2_pos - c.position <= 150) or (c.position < 5 and c.position > -10)) {
				c.move_flag = false;
			}
			else {
				c.position += move_length;
				if (c2_pos > c.position and c2_pos - c.position <= 140)
					c.position -= move_length;
			}
		}
			break;
		case 1:
		case 3:
			if (c.position > 0 and c.position < move_length+1) {
				c.position = 0;
				c.move_flag = false;
			}
			else if (c.position < -140 and c2_pos > c.position and c2_pos - c.position <= 150) {
				c.move_flag = false;
			}
			else if(c.move_flag){
				c.position += move_length;
				if (c2_pos > c.position and c2_pos - c.position <= 140)
					c.position -= move_length;
			}
			break;
		case 2:
			c.move_flag = true;
			c.position += move_length;
			if (c2_pos > c.position and c2_pos - c.position <= 140) {
				c.position -= move_length;
			}
			break;
		}

		if (i < 4) {
			if (c.position > (width - length_w))
				c.position = -length_w;
		}
		else {
			if (c.position > (height - length_h))
				c.position = -length_h;
		}
	}

	if(traffic_stop_flag) {
		bool all_stop_flag{ true };
		for (int i{}; i < 8; i++) {
			if (car[i].position != prev_positions[i]) {
				all_stop_flag = false;
				break;
			}
		}

		if (all_stop_flag and player_move_flag) {
			int flag{ dis(gen) };

			switch (player_move) {
			case 1:
			case 4:
				p.x += 1 * 4;
				p.y += 1 * 4;

				if (p.x >= 400 and p.y >= 400) {
					p.x = 400;
					p.y = 400;
					player_move = flag ? -3 : -2;
					player_move_flag = false;
				}
				break;
			case 2:
			case -1:
				p.x += 1 * 4;
				p.y += -1 * 4;

				if (p.x >= 400 and p.y <= 0) {
					p.x = 400;
					p.y = 0;
					player_move = flag ? -4 : 3;
					player_move_flag = false;
				}
				break;
			case -2:
			case -3:
				p.x += -1 * 4;
				p.y += -1 * 4;

				if (p.x <= 0 and p.y <= 0) {
					p.x = 0;
					p.y = 0;
					player_move = flag ? 4 : 1;
					player_move_flag = false;
				}
				break;
			case 3:
			case -4:
				p.x += -1 * 4;
				p.y += 1 * 4;

				if (p.x <= 0 and p.y >= 400) {
					p.x = 0;
					p.y = 400;
					player_move = flag ? 2 : -1;
					player_move_flag = false;
				}
				break;
			}
		}
	}else{ // 사람 이동
		int flag{ dis(gen) };
		//int flag{ 0 };

		if (traffic_light == 2) {
			switch (player_move) {
			case 1:
			case -1:
				if (player_move_flag)
					p.y += player_move * 4;

				if (player_move == 1 and p.y >= 400) {
					p.y = 400;
					player_move = flag ? 2 : -1;
					player_move_flag = false;
				}
				else if (player_move == -1 and p.y <= 0) {
					p.y = 0;
					player_move = flag ? 4 : 1;
					player_move_flag = false;
				}
				break;
			case 3:
			case -3:
				if (player_move_flag)
					p.y += player_move / 3 * 4;

				if (player_move == 3 and p.y >= 400) {
					p.y = 400;
					player_move = flag ? -2 : -3;
					player_move_flag = false;
				}
				else if (player_move == -3 and p.y <= 0) {
					p.y = 0;
					player_move = flag ? -4 : 3;
					player_move_flag = false;
				}
				break;
			}
		}
		else if (traffic_light == 0) {
			switch (player_move) {
			case 2:
			case -2:
				if (player_move_flag)
					p.x += player_move / 2 * 4;

				if (player_move == 2 and p.x >= 400) {
					p.x = 400;
					player_move = flag ? -3 : -2;
					player_move_flag = false;
				}
				else if (player_move == -2 and p.x <= 0) {
					p.x = 0;
					player_move = flag ? -1 : 2;
					player_move_flag = false;
				}
				break;
			case 4:
			case -4:
				if (player_move_flag)
					p.x += player_move / 4 * 4;

				if (player_move == 4 and p.x >= 400) {
					p.x = 400;
					player_move = flag ? 3 : -4;
					player_move_flag = false;
				}
				else if (player_move == -4 and p.x <= 0) {
					p.x = 0;
					player_move = flag ? 1 : 4;
					player_move_flag = false;
				}
				break;
			}
		}
	}
}