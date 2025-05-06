#pragma once
#include <Windows.h>
#include <vector>
#include "Cell.h"

void GRID_PAINT(HDC& hdc, RECT& f_rect, Cell board[ROW][COL]) {
    COLORREF color[]{ RGB(255, 60, 60), RGB(63, 73, 204), RGB(251, 237, 68), 
        RGB(54, 224, 18), RGB(26, 24, 21)};

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

    float rect_width{ (float)(f_rect.right - f_rect.left)/COL };
    float rect_height{ (float)(f_rect.bottom - f_rect.top) / ROW };

    for (int i{}; i < ROW; i++) {
        for (int j{}; j < COL; j++) {
            RECT rect{ f_rect.left, f_rect.top, f_rect.left + rect_width, f_rect.top + rect_height };
            OffsetRect(&rect, rect_width * i, rect_width * j);
            InflateRect(&rect, -4, -4);

            int centerx{ (rect.left + rect.right) / 2 };
            int centery{ (rect.top + rect.bottom) / 2 };

            Cell& c = board[i][j];

            if (not c.open_flag and not c.hint_flag) {
                continue;
            }

            switch (c.type) {
            case 0:
                hbrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
                InflateRect(&rect, 3, 3);
                FillRect(hdc, &rect, hbrush);
                break;
            case 1:
            {
                hbrush = CreateSolidBrush(RGB(0, 0, 0));
                oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

                SelectObject(hdc, oldbrush);
                DeleteObject(hbrush);
                break;
            }
            case 2:
            {
                hbrush = CreateSolidBrush(RGB(255, 0, 0));
                oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

                POINT pt[4]{
                    {centerx, rect.top},
                    {rect.right, centery},
                    {centerx, rect.bottom},
                    {rect.left, centery}
                };
                Polygon(hdc, pt, 4);

                SelectObject(hdc, oldbrush);
                DeleteObject(hbrush);
                break;
            }
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                hbrush = CreateSolidBrush(color[c.type - 3]);
                oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

                switch (c.piece) {
                case 0:
                    Pie(hdc, rect.left, rect.top, rect.right, rect.bottom, centerx, rect.top, rect.left, centery);
                    break;
                case 1:
                    Pie(hdc, rect.left, rect.top, rect.right, rect.bottom, rect.left, centery, centerx, rect.bottom);
                    break;
                case 2:
                    Pie(hdc, rect.left, rect.top, rect.right, rect.bottom, centerx, rect.bottom, rect.right, centery);
                    break;
                case 3:
                    Pie(hdc, rect.left, rect.top, rect.right, rect.bottom, rect.right, centery, centerx, rect.top);
                    break;
                }

                SelectObject(hdc, oldbrush);
                DeleteObject(hbrush);
                break;
            }
        }
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

}