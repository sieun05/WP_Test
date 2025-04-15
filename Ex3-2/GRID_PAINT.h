#pragma once
#include <windows.h>
#include <vector>
#include "Circle.h"
#include "Shape_Paint.h"

#define COLUMN 20
#define ROW 20

void GRID_PAINT(HDC& hdc, RECT& full_rect, int board[ROW][COLUMN], std::vector<Circle>& shapes) {
    COLORREF color[]{RGB(247, 213, 156), RGB(154, 219, 138), RGB(134, 192, 226), RGB(235, 117, 132), RGB(132, 112, 238), RGB(226, 153, 230)};

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_height{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{}; i < ROW; i++) {
        for (int j{}; j < COLUMN; j++) {
            if (board[i][j] == 1) {
                RECT rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
                OffsetRect(&rect, rect_width * j, rect_height * i);

                FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 0)));
            }
        }
    }

    for (int i{ 1 }; i < COLUMN; i++) { //세로줄
        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }

    for (int i{ 1 }; i < ROW; i++) { //가로줄
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_height * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_height * i);
    }

    for (int i{}; i < shapes.size(); i++) {
        Circle& s = shapes[i];

        RECT rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
        OffsetRect(&rect, rect_width * s.x, rect_height * s.y);
        if(s.order>0)
            InflateRect(&rect, (s.order * -1), (s.order * -1));
        else
            InflateRect(&rect, s.size - 2, s.size - 2);

		HBRUSH hBrush = CreateSolidBrush(color[s.color]);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Circle_Paint(hdc, rect); //원 그리기

		SelectObject(hdc, oldBrush);
        DeleteObject(oldBrush);
    }

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
