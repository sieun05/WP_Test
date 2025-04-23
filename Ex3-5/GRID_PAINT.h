#pragma once
#include <Windows.h>
#include <vector>

#define COLUMN 30
#define ROW 30

void GRID_PAINT(HDC& hdc, RECT& f_Rect) {

	HPEN hpen{}, oldpen{};
	int carline_width{ 300 };
	int pline_width{ 100 };

	COLORREF color{};
	HBRUSH hbrush{};

	//차선
	{
		hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);
		color = { RGB(82, 82, 82) };
		hbrush = CreateSolidBrush(color);

		/*RECT c_rect{
			f_Rect.left,
			f_Rect.bottom/2-carline_width/2,
			f_Rect.right,
			f_Rect.bottom / 2+carline_width / 2,
		};

		FillRect(hdc, &c_rect, hbrush);

		c_rect={
			f_Rect.right / 2 - carline_width/2,
			f_Rect.top,
			f_Rect.right / 2 + carline_width/2,
			f_Rect.bottom,
		};

		FillRect(hdc, &c_rect, hbrush);*/

		//세로줄
		MoveToEx(hdc, f_Rect.right / 2 - carline_width / 2, f_Rect.top, NULL);
		LineTo(hdc, f_Rect.right / 2 - carline_width / 2, f_Rect.bottom);
		MoveToEx(hdc, f_Rect.right / 2 + carline_width / 2, f_Rect.top, NULL);
		LineTo(hdc, f_Rect.right / 2 + carline_width / 2, f_Rect.bottom);

		//가로줄
		MoveToEx(hdc, f_Rect.left, f_Rect.bottom / 2 - carline_width / 2, NULL);
		LineTo(hdc, f_Rect.right, f_Rect.bottom / 2 - carline_width / 2);
		MoveToEx(hdc, f_Rect.left, f_Rect.bottom / 2 + carline_width / 2, NULL);
		LineTo(hdc, f_Rect.right, f_Rect.bottom / 2 + carline_width / 2);

		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
	}


	//중앙 선
	{
		hpen = CreatePen(PS_DOT, 2, RGB(255, 255, 25));
		oldpen = (HPEN)SelectObject(hdc, hpen);

		MoveToEx(hdc, f_Rect.right / 2, f_Rect.top, NULL);
		LineTo(hdc, f_Rect.right / 2, f_Rect.bottom);

		MoveToEx(hdc, f_Rect.left, f_Rect.bottom / 2, NULL);
		LineTo(hdc, f_Rect.right, f_Rect.bottom / 2);

		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
	}

	//인도
	{
		hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);

		//횡단보도 세로
		{
			RECT p_rect{
				f_Rect.right / 2 - carline_width / 2 - pline_width,
				f_Rect.bottom / 2 - carline_width / 2,
				f_Rect.right / 2 - carline_width / 2,
				f_Rect.bottom / 2 - carline_width / 2 + 40 };
			OffsetRect(&p_rect, 0, 10);

			color = { RGB(241, 248, 255) };
			hbrush = CreateSolidBrush(color);

			for (int i{}; i < 5; i++) {
				FillRect(hdc, &p_rect, hbrush);
				OffsetRect(&p_rect, 0, 60);
			}
		}
		{
			RECT p_rect{
				f_Rect.right / 2 - carline_width / 2 - pline_width,
				f_Rect.bottom / 2 - carline_width / 2,
				f_Rect.right / 2 - carline_width / 2,
				f_Rect.bottom / 2 - carline_width / 2 + 40 };
			OffsetRect(&p_rect, 401, 10);

			for (int i{}; i < 5; i++) {
				FillRect(hdc, &p_rect, hbrush);
				OffsetRect(&p_rect, 0, 60);
			}
		}

		//횡단보도 가로
		{
			RECT p_rect{
				f_Rect.right / 2 - carline_width / 2,
				f_Rect.bottom / 2 - carline_width / 2 - pline_width,
				f_Rect.right / 2 - carline_width / 2 + 40,
				f_Rect.bottom / 2 - carline_width / 2};
			OffsetRect(&p_rect, 10, 0);

			for (int i{}; i < 5; i++) {
				FillRect(hdc, &p_rect, hbrush);
				OffsetRect(&p_rect, 60, 0);
			}
		}
		{
			RECT p_rect{
				f_Rect.right / 2 - carline_width / 2,
				f_Rect.bottom / 2 - carline_width / 2 - pline_width,
				f_Rect.right / 2 - carline_width / 2 + 40,
				f_Rect.bottom / 2 - carline_width / 2 };
			OffsetRect(&p_rect, 10, 401);

			for (int i{}; i < 5; i++) {
				FillRect(hdc, &p_rect, hbrush);
				OffsetRect(&p_rect, 60, 0);
			}
		}

		//세로줄
		MoveToEx(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.top, NULL);
		LineTo(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.bottom);
		MoveToEx(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.top, NULL);
		LineTo(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.bottom);

		//가로줄
		MoveToEx(hdc, f_Rect.left, f_Rect.bottom / 2 - carline_width / 2 - pline_width, NULL);
		LineTo(hdc, f_Rect.right, f_Rect.bottom / 2 - carline_width / 2 - pline_width);
		MoveToEx(hdc, f_Rect.left, f_Rect.bottom / 2 + carline_width / 2 + pline_width, NULL);
		LineTo(hdc, f_Rect.right, f_Rect.bottom / 2 + carline_width / 2 + pline_width);

		SelectObject(hdc, oldpen);
		DeleteObject(hpen);

		hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		oldpen = (HPEN)SelectObject(hdc, hpen);

		MoveToEx(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.bottom/2-carline_width/2 - pline_width, NULL);
		LineTo(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.bottom/2 + carline_width / 2 + pline_width);
		MoveToEx(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.bottom / 2 - carline_width / 2 - pline_width, NULL);
		LineTo(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.bottom / 2 + carline_width / 2 + pline_width);

		SelectObject(hdc, oldpen);
		DeleteObject(hpen);

		hpen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);

		MoveToEx(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.bottom / 2 - carline_width / 2, NULL);
		LineTo(hdc, f_Rect.right / 2 - carline_width / 2 - pline_width, f_Rect.bottom / 2 + carline_width / 2 + pline_width);
		MoveToEx(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.bottom / 2 - carline_width / 2 - pline_width, NULL);
		LineTo(hdc, f_Rect.right / 2 + carline_width / 2 + pline_width, f_Rect.bottom / 2 + carline_width / 2);

		SelectObject(hdc, oldpen);
		DeleteObject(hpen);
	}

	

}