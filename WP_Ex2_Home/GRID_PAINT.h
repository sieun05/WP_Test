#pragma once
#include "head.h"

#define COLUMN 10
#define ROW 10

void GRID_PAINT(HDC& hdc, RECT& full_rect, int board[ROW][COLUMN]) {

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_heigth{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 0}; i < COLUMN; i++) { //세로줄
        for (int j{}; j < ROW; j++) {
            if (board[j][i] == 1) {
                RECT sell_rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_heigth };
                OffsetRect(&sell_rect, rect_width * i + 1, rect_heigth * j + 1);
                InflateRect(&sell_rect, -3, -3);

                FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(255, 0, 0)));
            }
        }

        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }

    for (int i{ 1 }; i < ROW; i++) { //가로줄
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_heigth * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_heigth * i);

        
    }

    /*RECT sell_rect{ full_rect.left, full_rect.top, full_rect.left + rect_width -1, full_rect.top + rect_heigth -1};
    OffsetRect(&sell_rect, rect_width*2 +1, rect_heigth*0+1);

    FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(50, 50, 150)));*/

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

void GRID_PAINT(HDC& hdc, RECT& full_rect, HPEN& hPen) {

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_heigth{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 1 }; i < COLUMN; i++) {
        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }

    for (int i{ 1 }; i < ROW; i++) {
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_heigth * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_heigth * i);
    }

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}