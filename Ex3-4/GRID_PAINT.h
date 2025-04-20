#pragma once
#include <windows.h>
#include <vector>
#include "Cell.h"
#include "Shape.h"

#define COLUMN 20
#define ROW 25

void GRID_PAINT(HDC& hdc, RECT& full_rect, Cell board[ROW][COLUMN], const std::vector<Block>& blocks) {
    COLORREF color[]{ RGB(247, 213, 156), RGB(154, 219, 138), RGB(134, 192, 226), RGB(235, 117, 132), RGB(132, 112, 238), RGB(226, 153, 230) };

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_height{ (float)(full_rect.bottom - full_rect.top) / ROW };

   
    for (const Block& b : blocks) {
        for (int j{ b.x }; j < b.x + b.width; j++) {
            for (int i{ b.y }; i < b.y + b.height; i++) {
                RECT rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
                OffsetRect(&rect, rect_width * j, rect_height * i);

                HBRUSH hbrush = CreateSolidBrush(color[b.color]);
                FillRect(hdc, &rect, hbrush);
                DeleteObject(hbrush);
            }
        }
    }

    for (int i{}; i < ROW; i++) {
        for (int j{}; j < COLUMN; j++) {
            if(board[i][j].type){
                RECT rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
                OffsetRect(&rect, rect_width * j, rect_height * i);
                const Cell& cell = board[i][j];

                HBRUSH hbrush = CreateSolidBrush(color[cell.color]);
                FillRect(hdc, &rect, hbrush);
                DeleteObject(hbrush);
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

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
