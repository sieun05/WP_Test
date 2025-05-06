#pragma once
#include <Windows.h>
#include <vector>
#include "Cell.h"
#include "Rect.h"


void GRID_PAINT(HDC& hdc, const RECT& f_rect, const Cell board[ROW][COL], 
    int Grid_style, std::vector<Rect>& shapes, int select_index) {
    COLORREF color[]{ RGB(255, 60, 60), RGB(63, 73, 204), RGB(251, 237, 68), RGB(54, 224, 18), RGB(26, 24, 21), RGB(226, 239, 239) };
	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	float rect_width{ (float)(f_rect.right - f_rect.left) / COL };
	float rect_height{ (float)(f_rect.bottom - f_rect.top) / ROW };

    if (Grid_style==1) {
        hpen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);
    }
    else if (Grid_style == 2) {
        hpen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);
    }
    //세로줄
    for (int i{ 1 }; i < COL; i++) {
        MoveToEx(hdc, f_rect.left + rect_width * i, f_rect.top, NULL);
        LineTo(hdc, f_rect.left + rect_width * i, f_rect.bottom);
    }
    //가로줄
    for (int i{ 1 }; i < ROW; i++) {
        MoveToEx(hdc, f_rect.left, f_rect.top + rect_height * i, NULL);
        LineTo(hdc, f_rect.right, f_rect.top + rect_height * i);
    }
    FrameRect(hdc, &f_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
    if (Grid_style) {
        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
    }

    for (int i{}; i < shapes.size(); i++) {
        Rect& r = shapes.at(i);
        if (r.DB_click_flag) continue;


        // 시작 지점 (왼쪽 위 좌표)
        int left = f_rect.left + rect_width * r.sx;
        int top = f_rect.top + rect_height * r.sy;

        // 끝 지점 (오른쪽 아래 좌표)
        int right = f_rect.left + rect_width * r.ex;
        int bottom = f_rect.top + rect_height * r.ey;

        RECT rect = { left, top, right, bottom };

        hbrush = CreateSolidBrush(color[r.color-1]);
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

        if(select_index==i)
            hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        else
            hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);

        if (r.border or select_index==i) {
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom); 
        }
        else {
            FillRect(hdc, &rect, hbrush);
        }

        SelectObject(hdc, oldbrush);
        DeleteObject(hbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
    }

    for (int i{}; i < shapes.size(); i++) {
        Rect& r = shapes.at(i);
        if (not r.DB_click_flag) continue;


        // 시작 지점 (왼쪽 위 좌표)
        int left = f_rect.left + rect_width * r.sx;
        int top = f_rect.top + rect_height * r.sy;

        // 끝 지점 (오른쪽 아래 좌표)
        int right = f_rect.left + rect_width * r.ex;
        int bottom = f_rect.top + rect_height * r.ey;

        RECT rect = { left, top, right, bottom };

        hbrush = CreateSolidBrush(color[r.color - 1]);
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

        if (select_index == i)
            hpen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
        else
            hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);

        if (r.border or select_index == i) {
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        }
        else {
            FillRect(hdc, &rect, hbrush);
        }

        SelectObject(hdc, oldbrush);
        DeleteObject(hbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
    }
}