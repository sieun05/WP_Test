#pragma once
#include <Windows.h>
#include "Car.h"

void GRID_PAINT(HDC& hdc, RECT& f_rect, const Car car[], int traffic_light, const POINT& p, RECT player) {
    COLORREF light_color[3]{RGB(255,20,20), RGB(255, 255, 40), RGB(20, 255, 20)};
    HBRUSH hbrush{}, oldbrush{};
    HPEN hpen{}, oldpen{};
    int centerX{ (f_rect.left + f_rect.right) / 2 };
    int centerY{ (f_rect.top + f_rect.bottom) / 2 };
    int width{ f_rect.right-f_rect.left };
    int height{ f_rect.bottom - f_rect.top };

    //가운데노란선
    {
        hpen = CreatePen(PS_DASH, 1, RGB(255, 210, 20));
        oldpen = (HPEN)SelectObject(hdc, hpen);

        MoveToEx(hdc, f_rect.left, centerY, NULL);
        LineTo(hdc, f_rect.right, centerY);
        MoveToEx(hdc, centerX, f_rect.top, NULL);
        LineTo(hdc, centerX, f_rect.bottom);

        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
    }

    //차선
    {
        hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);

        MoveToEx(hdc, f_rect.left, centerY - 150, NULL);
        LineTo(hdc, f_rect.right, centerY - 150);
        MoveToEx(hdc, f_rect.left, centerY + 150, NULL);
        LineTo(hdc, f_rect.right, centerY + 150);

        MoveToEx(hdc, centerX - 150, f_rect.top, NULL);
        LineTo(hdc, centerX - 150, f_rect.bottom);
        MoveToEx(hdc, centerX + 150, f_rect.top, NULL);
        LineTo(hdc, centerX + 150, f_rect.bottom);

        SelectObject(hdc, oldpen);
        DeleteObject(hpen);


        hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        oldpen = (HPEN)SelectObject(hdc, hpen);

        MoveToEx(hdc, centerX - 150, centerY - 150 - 100, NULL);
        LineTo(hdc, centerX + 150, centerY - 150 - 100);
        MoveToEx(hdc, centerX - 150, centerY + 150 + 100, NULL);
        LineTo(hdc, centerX + 150, centerY + 150 + 100);

        MoveToEx(hdc, centerX - 150 - 100, centerY - 150, NULL);
        LineTo(hdc, centerX - 150 - 100, centerY + 150);
        MoveToEx(hdc, centerX + 150 + 100, centerY - 150, NULL);
        LineTo(hdc, centerX + 150 + 100, centerY + 150);

        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
    }

    //신호등
    {
        int traffic_light_size{ 70 };

        for (int k{}; k<2; k++){
            RECT traffic_rect{};
            int n = (traffic_light == 3) ? 1 : traffic_light;

            if (k == 0)
                traffic_rect={ 10, 10, 10 + traffic_light_size * 3, 10 + traffic_light_size };
            else
                traffic_rect = { f_rect.right -10 - traffic_light_size * 3, f_rect.bottom - 10 - traffic_light_size, f_rect.right - 10, f_rect.bottom - 10 };

            FrameRect(hdc, &traffic_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

            for (int i{}; i < 3; i++) {
                RECT traffic_circle{ traffic_rect.left, traffic_rect.top,
                    traffic_rect.left + traffic_light_size, traffic_rect.top + traffic_light_size };
                OffsetRect(&traffic_circle, traffic_light_size * i, 0);
                InflateRect(&traffic_circle, -7, -7);

                hbrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
                oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

                Ellipse(hdc, traffic_circle.left, traffic_circle.top, traffic_circle.right, traffic_circle.bottom);

                SelectObject(hdc, oldbrush);
            }

            RECT traffic_circle{ traffic_rect.left, traffic_rect.top,
                    traffic_rect.left + traffic_light_size, traffic_rect.top + traffic_light_size };

            if (k == 0) {
                hbrush = CreateSolidBrush(light_color[n]);
                OffsetRect(&traffic_circle, traffic_light_size * n, 0);
            }
            else {
                hbrush = CreateSolidBrush(light_color[2 - n]);
                OffsetRect(&traffic_circle, traffic_light_size * (2- n), 0);
            }
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
            InflateRect(&traffic_circle, -7, -7);

            Ellipse(hdc, traffic_circle.left, traffic_circle.top, traffic_circle.right, traffic_circle.bottom);

            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }
    }

    //차
    {
        for (int i{}; i < 8; i++) {
            //if (i % 2 == 1) continue;

            RECT car_rect{};
            const Car& c = car[i];
            int car_width{ 130 };
            int car_height{ 60 };
            int padding{ (150 - car_height) / 2 };

            int over{};

            switch (c.direction) {
            case -1: //좌
                car_rect = {
                    centerX + 150 + 100, centerY - 150 + padding, 
                    centerX + 150 + 100 + car_width, centerY - 150 + padding + car_height };
                OffsetRect(&car_rect, c.position * c.direction, 0);
                over = car_rect.right - f_rect.right; //양수면 빠진거
                break;
            case 1: //우
                car_rect = { 
                    centerX - 150 - 100 - car_width, centerY + padding,
                    centerX - 150 - 100, centerY + padding + car_height };
                OffsetRect(&car_rect, c.position* c.direction, 0);
                over = f_rect.left - car_rect.left;
                break;
            case -2: //위
                car_rect = {
                    centerX + padding, centerY + 150 + 100,
                    centerX + padding + car_height, centerY + 150 + 100 + car_width };
                OffsetRect(&car_rect, 0, c.position* (c.direction/2));
                over = car_rect.bottom - f_rect.bottom;
                break;
            case 2: //아래
                car_rect = {
                    centerX - 150 + padding, centerY - 150 - 100 - car_width,
                    centerX -150 + padding + car_height, centerY - 150 - 100 };
                OffsetRect(&car_rect, 0, c.position* (c.direction/2));
                over = f_rect.top - car_rect.top;
                break;
            }

            hbrush = CreateSolidBrush(RGB(186, 223, 226));
            oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

            Rectangle(hdc, car_rect.left, car_rect.top, car_rect.right, car_rect.bottom);
            TCHAR txt[100];
           
            if (over > 0) {
                switch (c.direction) {
                case -1:
                case 1:
                    OffsetRect(&car_rect, c.direction*width, 0);
                    break;
                case 2:
                case -2:
                    OffsetRect(&car_rect, 0, (c.direction/2) * height);
                    break;
                }
            }
            Rectangle(hdc, car_rect.left, car_rect.top, car_rect.right, car_rect.bottom);

            wsprintf(txt, TEXT("%d\n%d %d"), i, c.direction, c.position);
            DrawText(hdc, txt, lstrlen(txt), &car_rect, DT_CENTER | DT_VCENTER);

            SelectObject(hdc, oldbrush);
            DeleteObject(hbrush);
        }
    
    }

    //사람
    {
        hbrush = CreateSolidBrush(RGB(243, 232, 129));
        oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

        OffsetRect(&player, p.x, p.y);
        InflateRect(&player, -4, -4);
        Ellipse(hdc, player.left, player.top, player.right, player.bottom);

        SelectObject(hdc, oldbrush);
        DeleteObject(hbrush);
    }
}