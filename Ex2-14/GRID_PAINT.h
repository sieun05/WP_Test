#pragma once
#include <windows.h>
#include <vector>
#include "Shape.h"

#define COLUMN 40
#define ROW 40

void GRID_PAINT(HDC& hdc, RECT& full_rect, int board[ROW][COLUMN], const std::vector<Shape>& shapes, const int& select_index) {

    float rect_width{ (float)(full_rect.right - full_rect.left) / COLUMN };
    float rect_height{ (float)(full_rect.bottom - full_rect.top) / ROW };

    for (int i{ 1 }; i < ROW; i++) { //가로줄
        MoveToEx(hdc, full_rect.left, full_rect.top + rect_height * i, NULL);
        LineTo(hdc, full_rect.right, full_rect.top + rect_height * i);

        MoveToEx(hdc, full_rect.left + rect_width * i, full_rect.top, NULL);
        LineTo(hdc, full_rect.left + rect_width * i, full_rect.bottom);
    }

    //for (int i{ 1 }; i<COLUMN; i++){
    //    
    //}

    for (int i{ 0 }; i < COLUMN; i++) { //세로줄
        for (int j{}; j < ROW; j++) {

            RECT sell_rect{ full_rect.left, full_rect.top, full_rect.left + rect_width, full_rect.top + rect_height };
            OffsetRect(&sell_rect, rect_width * i + 1, rect_height * j + 1);

            HBRUSH hbrush{}, old_brush{};
            HPEN hpen{}, old_pen{};

            if (board[j][i]) {
                int shape_index = board[j][i] - 1;
                if (shape_index >= 0 and shape_index < shapes.size()) {
                    const Shape& s = shapes[shape_index];
                    InflateRect(&sell_rect, s.size, s.size);

                    if (shape_index == select_index) {
                        
                        hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                        old_pen = (HPEN)SelectObject(hdc, hpen);
                    }
                    else {
                        hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                        old_pen = (HPEN)SelectObject(hdc, hpen);
                    }

                    if (s.flag_c) {
                        hbrush = CreateSolidBrush(RGB(60, 255, 55));
                        old_brush = (HBRUSH)SelectObject(hdc, hbrush);

                        POINT point_p[10]{
                            {(sell_rect.left + sell_rect.right) / 2., sell_rect.top},
                            {sell_rect.right ,(sell_rect.top + sell_rect.bottom) / 2. - 2},
                            {sell_rect.right - 6, sell_rect.bottom},
                            {sell_rect.left + 6, sell_rect.bottom},
                            {sell_rect.left ,(sell_rect.top + sell_rect.bottom) / 2. - 2} };
                        Polygon(hdc, point_p, 5);
                    }
                    else {
                        hbrush = CreateSolidBrush(RGB(s.r, s.g, s.b));
                        old_brush = (HBRUSH)SelectObject(hdc, hbrush);

                        switch (s.shape_sel) {
                        case 1:
                        {
                            Ellipse(hdc, sell_rect.left, sell_rect.top, sell_rect.right, sell_rect.bottom);
                            break;
                        }
                        case 2:
                        {
                            POINT point[6]{ {sell_rect.right - (sell_rect.right - sell_rect.left) / 2., sell_rect.top}, { sell_rect.left, sell_rect.bottom }, { sell_rect.right, sell_rect.bottom } };
                            Polygon(hdc, point, 3);
                            break;
                        }
                        case 3:
                        {
                            POINT point[8]{
                                {sell_rect.left, sell_rect.top},
                                {sell_rect.right, sell_rect.top},
                                {sell_rect.right, sell_rect.bottom},
                                {sell_rect.left, sell_rect.bottom}
                            };
                            Polygon(hdc, point, 4);
                            break;
                        }
                        }
                    }

                    SelectObject(hdc, old_brush);
                    SelectObject(hdc, old_pen);
                    DeleteObject(hpen);
                    DeleteObject(hbrush);
                } 
            }
        }   
    }
    FrameRect(hdc, &full_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
