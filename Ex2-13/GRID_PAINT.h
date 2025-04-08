#pragma once
#include <windows.h>
#include "Player.h"

#define COLUMN 20
#define ROW 20

void GRID_PAINT(HDC& hdc, RECT& full_rect, int board[ROW][COLUMN], const Player& p) {

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_height{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 0 }; i < COLUMN; i++) { //세로줄
        for (int j{}; j < ROW; j++) {
            RECT sell_rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
            OffsetRect(&sell_rect, rect_width * i + 1, rect_height* j + 1);

            HFONT hfont = CreateFont(rect_height-10, 0, 0, 100, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0,
                VARIABLE_PITCH | FF_ROMAN, TEXT("ONE Mobile POP"));
            HFONT old_font = (HFONT)SelectObject(hdc, hfont);

            if (board[j][i] <= 'Z' and board[j][i] >= 'A') {
                TCHAR prt_txt[100]{};
                wsprintf(prt_txt, TEXT("%c"), board[j][i]);

                DrawText(hdc, prt_txt, -1, &sell_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            if (j == p.y and i == p.x) {
                HBRUSH hbrush = CreateSolidBrush(RGB(255, 195, 55));
                HBRUSH old_brush = (HBRUSH)SelectObject(hdc, hbrush);

                HPEN hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
                HPEN old_pen = (HPEN)SelectObject(hdc, hpen);

                InflateRect(&sell_rect, -5, -5);
                Ellipse(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom);

                SelectObject(hdc, old_brush);
                SelectObject(hdc, old_pen);

                DeleteObject(hbrush);
                DeleteObject(hpen);
            }

            SelectObject(hdc, old_font);
            DeleteObject(hfont);
        }

        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }

    for (int i{ 1 }; i < ROW; i++) { //가로줄
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_height * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_height * i);
    }

    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
