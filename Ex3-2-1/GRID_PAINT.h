#pragma once
#include <windows.h>
#include <vector>
#include "Circle.h"
#include "Shape_Paint.h"
#include "Cell.h"

#define COLUMN 40
#define ROW 40

void GRID_PAINT(HDC& hdc, RECT& full_rect, Cell board[ROW][COLUMN], std::vector<Circle>& shapes) {
    COLORREF color[]{ RGB(247, 213, 156), RGB(154, 219, 138), RGB(134, 192, 226), RGB(235, 117, 132), RGB(132, 112, 238), RGB(226, 153, 230) };

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_height{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{}; i < ROW; i++) {
        for (int j{}; j < COLUMN; j++) {
            RECT rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
            OffsetRect(&rect, rect_width * j, rect_height * i);

            switch (board[i][j].type) {
            case 1: //장애물
            {
                HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
                FillRect(hdc, &rect, redBrush);
                DeleteObject(redBrush);
                break;
            }
            case 2: //먹이들
            {
                InflateRect(&rect, -5, -5);

                HBRUSH hBrush = CreateSolidBrush(color[board[i][j].color]);
                //HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                //Circle_Paint(hdc, rect);
                //SelectObject(hdc, oldBrush);

                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
                break;
            }
            }

        }
    }

    //세로줄
    for (int i{ 1 }; i < COLUMN; i++) {
        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }
    //가로줄
    for (int i{ 1 }; i < ROW; i++) {
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_height * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_height * i);
    }

    //원들

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
