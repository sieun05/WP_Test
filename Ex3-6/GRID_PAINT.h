#pragma once
#include <Windows.h>
#include <vector>
#include "Obstacle.h"

#define R1_ROW 6
#define R1_COL 12
#define R2_ROW 8
#define R2_COL 8

void GRID_PAINT(
	HDC& hdc, const RECT& f_rect, const RECT player_field[], const int board[R2_ROW][R2_COL], const int& round,
	const Obstacle obstacle[], const std::vector<Obstacle> R2_obstacle_pos, const POINT& p, const std::vector<POINT>& item) {

	COLORREF color[]{ RGB(230, 232, 255), RGB(255, 255, 255), RGB(173, 244, 178), RGB(60, 60, 255), RGB(255,40, 40), RGB(255,255, 50)};
	COLORREF rcolor[]{RGB(255, 148, 77), RGB(64,227,64), RGB(180, 70, 220), RGB(232, 60, 64)};

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	int centerX{ (f_rect.left + f_rect.right)/2 };
	int centerY{ (f_rect.top + f_rect.bottom) / 2 };
	int width{ f_rect.right - f_rect.left };
	int height{ f_rect.bottom - f_rect.top };
	
	switch (round) {
	case 1:
	{
		int rect_width{ (int)width / R1_COL };
		int rect_height{ (int)height / R1_ROW };

		//맵 테두리, 플레이어 start/end 영역
		{
			hbrush = CreateSolidBrush(color[2]);
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldpen = (HPEN)SelectObject(hdc, hpen);

			Rectangle(hdc, player_field[0].left, player_field[0].top, player_field[0].right, player_field[0].bottom);
			Rectangle(hdc, player_field[1].left, player_field[1].top, player_field[1].right, player_field[1].bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);

			hpen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
			oldpen = (HPEN)SelectObject(hdc, hpen);

			Rectangle(hdc, f_rect.left, f_rect.top, f_rect.right + 1, f_rect.bottom + 1);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);
		}

		//선 지우기
		{
			hpen = CreatePen(PS_SOLID, 6, color[2]);
			oldpen = (HPEN)SelectObject(hdc, hpen);


			MoveToEx(hdc, f_rect.left, f_rect.top + rect_height * 2 + 4, NULL);
			LineTo(hdc, f_rect.left, f_rect.top + rect_height * 4 - 4);
			MoveToEx(hdc, f_rect.right, f_rect.top + rect_height * 2 + 4, NULL);
			LineTo(hdc, f_rect.right, f_rect.top + rect_height * 4 - 4);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);
		}
		
		//플레이어 영역
		for (int i{}; i < R1_ROW; i++) {
			for (int j{}; j < R1_COL; j++) {
				RECT rect{ f_rect.left, f_rect.top, f_rect.left + rect_width, f_rect.top + rect_height };
				OffsetRect(&rect, rect_width * j, rect_height * i);

				if ((i % 2 == 0 and j % 2 == 0) or (i % 2 == 1 and j % 2 == 1))
					hbrush = CreateSolidBrush(color[0]);
				else
					hbrush = CreateSolidBrush(color[1]);
				//oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

				FillRect(hdc, &rect, hbrush);

				DeleteObject(hbrush);
			}
		}

		//장애물
		for (int i{}; i < 12; i++) {
			const Obstacle& ob = obstacle[i];

			RECT rect{ ob.x - 6, ob.y - 6, ob.x + 6, ob.y + 6 };
			if(ob.color==0)
				hbrush = CreateSolidBrush(color[3]);
			else
				hbrush = CreateSolidBrush(rcolor[ob.color-1]);
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

			Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);
		}

		break;
	}
	case 2:
	{
		int rect_width{ (int)width / R2_COL };
		int rect_height{ (int)height / R2_ROW };

		//선, 시작/끝지점
		{
			hbrush = CreateSolidBrush(color[2]);
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldpen = (HPEN)SelectObject(hdc, hpen);

			Rectangle(hdc, player_field[0].left, player_field[0].top, player_field[0].right, player_field[0].bottom);
			Rectangle(hdc, player_field[1].left, player_field[1].top, player_field[1].right, player_field[1].bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);

			/*hpen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
			oldpen = (HPEN)SelectObject(hdc, hpen);

			Rectangle(hdc, f_rect.left, f_rect.top, f_rect.right + 1, f_rect.bottom + 1);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);*/
		}

		//선 지우기
		{
			hpen = CreatePen(PS_SOLID, 6, color[2]);
			oldpen = (HPEN)SelectObject(hdc, hpen);

			MoveToEx(hdc, player_field[0].right, player_field[0].top, NULL);
			LineTo(hdc, player_field[0].right, player_field[0].bottom);
			MoveToEx(hdc, player_field[1].left, player_field[1].bottom, NULL);
			LineTo(hdc, player_field[1].right, player_field[1].bottom);

			SelectObject(hdc, oldpen);
			DeleteObject(hpen);
		}

		//플레이어 영역
		{
			for (int i{}; i < R2_ROW; i++) {
				for (int j{}; j < R2_COL; j++) {
					if (board[i][j]) {
						RECT rect{ f_rect.left, f_rect.top, f_rect.left + rect_width, f_rect.top + rect_height };
						OffsetRect(&rect, rect_width * j, rect_height * i);

						if ((i % 2 == 0 and j % 2 == 0) or (i % 2 == 1 and j % 2 == 1))
							hbrush = CreateSolidBrush(color[0]);
						else
							hbrush = CreateSolidBrush(color[1]);

						FillRect(hdc, &rect, hbrush);

						DeleteObject(hbrush);
					}
				}
			}
		}

		//장애물
		for (const Obstacle& pt : R2_obstacle_pos) {
			RECT r{ pt.x - 6, pt.y - 6, pt.x + 6, pt.y + 6 };
			if (pt.color == 0)
				hbrush = CreateSolidBrush(color[3]);
			else
				hbrush = CreateSolidBrush(rcolor[pt.color - 1]);
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

			Ellipse(hdc, r.left, r.top, r.right, r.bottom);

			SelectObject(hdc, oldbrush);
			DeleteObject(hbrush);
		}

		break;
	}
	}

	//item
	for (int i{}; i < item.size(); i++) {
		const POINT& p = item.at(i);

		RECT rect{ p.x - 5, p.y - 5, p.x + 5, p.y + 5 };

		hbrush = CreateSolidBrush(color[5]);
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);
	}

	//플레이어
	{
		RECT rect{ p.x, p.y, p.x + 20, p.y + 20 };

		hbrush = CreateSolidBrush(color[4]);
		oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
		hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);

		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

		SelectObject(hdc, oldbrush);
		DeleteObject(hbrush);
		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
	}
}