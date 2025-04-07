#pragma once
#include "head.h"

void MATRIX_PAINT(HDC& hdc, RECT& full_rect) {
    float rect_size_c{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_size_r{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 1 }; i < ROW; i++) {
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_size_r * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_size_r * i);
    }

    for (int i{ 1 }; i < COLUMN; i++) {
        MoveToEx(hdc, full_rect.left + rect_size_c * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_size_c * i, full_rect.bottom);
    }

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}