#pragma once
#include <Windows.h>
#include "resource.h"
#include "Cell.h"
#include "Feed.h"
#include "Virus.h"

#define ROW 18
#define COL 32

void GRID_PAINT(HDC& hdc, const RECT& f_rect, std::vector<Cell>& cell,
    std::vector<Feed>& feed, const int& screen) {
    COLORREF f_RGB[6]{RGB(243, 139, 46), RGB(65, 216, 133), RGB(221, 60, 64), RGB(164, 49, 232), RGB(13, 51, 206), RGB(53, 240, 213)};

    HBRUSH hbrush{}, oldbrush{};
    HPEN hpen{}, oldpen{};

	int centerX{ (f_rect.left + f_rect.right) / 2 };
	int centerY{ (f_rect.top + f_rect.bottom) / 2 };
	int width{ f_rect.right - f_rect.left };
	int height{ f_rect.bottom - f_rect.top };
	float rect_width{ (float)width / COL };
	float rect_height{ (float)height / ROW };

    //맵 그리드
    {
        hpen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
        oldpen = (HPEN)SelectObject(hdc, hpen);
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
        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
       
        hpen = CreatePen(PS_DASH, 2, RGB(255, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);
        hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);     // 내부 투명
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

        Rectangle(hdc, f_rect.left, f_rect.top, f_rect.right, f_rect.bottom);

        SelectObject(hdc, oldpen);
        DeleteObject(hpen);  
        SelectObject(hdc, oldbrush);
        DeleteObject(hbrush);
    }

    //먹이
    for (int i{}; i < feed.size(); i++) {
        Feed& f = feed.at(i);

        int size = f.size / 2;
        RECT rect{ f.x - size, f.y - size, f.x + size, f.y + size };

        hbrush = CreateSolidBrush(f_RGB[f.color]);
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
        oldpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));

        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(hdc, oldbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(hbrush);
    }

    if (screen == 0) return;

    //세포
    for (int i{}; i < cell.size(); i++) {
        Cell& c = cell.at(i);

        float ssize{ c.size / 2 };
        RECT rect{ c.x - ssize, c.y - ssize, c.x + ssize, c.y + ssize };

        hbrush = CreateSolidBrush(RGB(248, 213, 73));
        hpen = CreatePen(PS_SOLID, 2, RGB(236, 174, 13));
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
        oldpen = (HPEN)SelectObject(hdc, hpen);

        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(hdc, oldbrush);
        SelectObject(hdc, oldpen);
        DeleteObject(hbrush);
        DeleteObject(hpen);
    }
}