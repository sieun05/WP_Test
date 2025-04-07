#pragma once
#include "head.h"

#define COLUMN 40
#define ROW 40

void GRID_PAINT(HDC& hdc, RECT& full_rect, int board[ROW][COLUMN], Player p[]) {

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_heigth{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 0}; i < COLUMN; i++) { //세로줄
        for (int j{}; j < ROW; j++) {
            RECT sell_rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_heigth };
            OffsetRect(&sell_rect, rect_width * i + 1, rect_heigth * j + 1);

            if (board[j][i] == 3) {
                FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(255, 0, 0)));
            }

            if (board[j][i] == 4) {
                FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(60, 250, 250)));
            }

            if (board[j][i] == 5) {
                FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(200, 0, 200)));
            }

            if (board[j][i] == 6) {
                FillRect(hdc, &sell_rect, (HBRUSH)CreateSolidBrush(RGB(20, 200, 200)));
            }

            if (board[j][i] == 7) {
                InflateRect(&sell_rect, -2, -2);
                OffsetRect(&sell_rect, -1, -1);
                POINT point_tt[8]{ {sell_rect.left, sell_rect.top}, {sell_rect.right, sell_rect.top}, {sell_rect.left, sell_rect.bottom}, {sell_rect.right, sell_rect.bottom} };
                Polygon(hdc, point_tt, 4);
            }

            if (board[j][i] == 9) {
                InflateRect(&sell_rect, -2, -2);
                OffsetRect(&sell_rect, -1, -1);
                POINT point_tt[6]{ {sell_rect.right - (sell_rect.right - sell_rect.left) / 2., sell_rect.top}, {sell_rect.left, sell_rect.bottom}, {sell_rect.right, sell_rect.bottom}};
                Polygon(hdc, point_tt, 3);
            }

            if (board[j][i] == 8) {
                InflateRect(&sell_rect, -2, -2);
                OffsetRect(&sell_rect, -1, -1);
                POINT point_tt[6]{ {sell_rect.right - (sell_rect.right - sell_rect.left) / 2., sell_rect.bottom}, {sell_rect.left, sell_rect.top}, {sell_rect.right, sell_rect.top} };
                Polygon(hdc, point_tt, 3);
            }

            if (p[0].x==i and p[0].y==j) {
                InflateRect(&sell_rect, p[0].size, p[0].size);
                HBRUSH hBrush, old_brush;
                hBrush = CreateSolidBrush(RGB(p[0].r, p[0].g, p[0].b));
                old_brush = (HBRUSH)SelectObject(hdc, hBrush);
                
                if (p[0].shape == 0) {
                    Ellipse(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom);
                }
                else if (p[0].shape == 1) {
                    Rectangle(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom);
                }
                else if (p[0].shape == 2) {
                    POINT point_p[10]{ {(sell_rect.left + sell_rect.right) / 2., sell_rect.top}, {sell_rect.right ,(sell_rect.top + sell_rect.bottom) / 2. - 1}, {sell_rect.right - 3, sell_rect.bottom}, {sell_rect.left + 3, sell_rect.bottom}, {sell_rect.left ,(sell_rect.top + sell_rect.bottom) / 2. - 1} };
                    Polygon(hdc, point_p, 5);
                }
                else if (p[0].shape == 3) {
                    Pie(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom, (sell_rect.left + sell_rect.right) / 2., sell_rect.top, sell_rect.right, (sell_rect.top + sell_rect.bottom) / 2.);
                }

                SelectObject(hdc, old_brush);
                DeleteObject(hBrush);
            }

            if (p[1].x == i and p[1].y == j) {
                InflateRect(&sell_rect, p[1].size, p[1].size);
                Ellipse(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom);
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