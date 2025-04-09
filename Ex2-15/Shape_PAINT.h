#pragma once
#include <Windows.h>
#include <vector>
#include "Shape.h"

void Shape_PAINT(HDC hdc, const std::vector<Shape>& shapes, const int& select_index, const int& all_shapes_print) {
	COLORREF p_color[5]{ RGB(255, 255,255), RGB(50, 255,50), RGB(255, 50,50), RGB(50, 50, 255), RGB(255, 255, 50) };
	COLORREF b_color[5]{ RGB(255, 255,255), RGB(50, 255,50), RGB(255, 50,50), RGB(50, 50, 255), RGB(255, 255, 50) };
	
	if (all_shapes_print) {
		for (int i{}; i < shapes.size(); i++) {
			const Shape& s = shapes[i];

			RECT rect{s.x1, s.y1, s.x2, s.y2 };
			InflateRect(&rect, s.size, s.size);

			HPEN hpen = CreatePen(PS_SOLID, s.width, p_color[s.pen_color]);
			HPEN old_pen = (HPEN)SelectObject(hdc, hpen);
			HBRUSH hbrush = CreateSolidBrush(b_color[s.brush_color]);
			HBRUSH old_brush = (HBRUSH)SelectObject(hdc, hbrush);

			switch (s.shape_sel) {
			case 2:
				MoveToEx(hdc, rect.left, rect.top, NULL);
				LineTo(hdc, rect.right, rect.bottom);
				break;
			case 3:
			{
				POINT p[6]{
					{rect.right / 2 + rect.left / 2., rect.top},
					{rect.left, rect.bottom},
					{rect.right, rect.bottom} };
				Polygon(hdc, p, 3);
				break;
			}
			case 4:
			{
				POINT p[8]{
					{rect.left, rect.top},
					{rect.left, rect.bottom},
					{rect.right, rect.bottom},
					{rect.right, rect.top} };
				Polygon(hdc, p, 4);
				break;
			}
			case 5:
			{
				POINT p[10]{
					{(rect.left + rect.right) / 2., rect.top},
					{rect.right ,(rect.top + rect.bottom) / 2. - (rect.right + rect.left) / 10},
					{rect.right - (rect.right + rect.left) / 3, rect.bottom},
					{rect.left + (rect.right + rect.left) / 3, rect.bottom},
					{rect.left ,(rect.top + rect.bottom) / 2. - (rect.right + rect.left) / 10} };
				Polygon(hdc, p, 5);
				break;
			}
			case 6:
			{
				POINT p[12]{
					{ (rect.left + rect.right) / 2., rect.top },
					{ rect.left, (rect.top + rect.bottom) / 2. - (rect.top + rect.bottom) / 4.},
					{ rect.left, (rect.top + rect.bottom) / 2. + (rect.top + rect.bottom) / 4. },
					{ (rect.left + rect.right) / 2., rect.bottom },
					{ rect.right, (rect.top + rect.bottom) / 2. + (rect.top + rect.bottom) / 4. },
					{ rect.right, (rect.top + rect.bottom) / 2. - (rect.top + rect.bottom) / 4. }
				};
				break;
			}
			}

			SelectObject(hdc, old_pen);
			SelectObject(hdc, old_brush);
			DeleteObject(hpen);
			DeleteObject(hbrush);
		}
	}
	else {
		const Shape& s = shapes[select_index];

		RECT rect{ s.x1, s.y1, s.x2, s.y2 };
		InflateRect(&rect, s.size, s.size);

		HPEN hpen = CreatePen(PS_SOLID, s.width, p_color[s.pen_color]);
		HPEN old_pen = (HPEN)SelectObject(hdc, hpen);
		HBRUSH hbrush = CreateSolidBrush(b_color[s.brush_color]);
		HBRUSH old_brush = (HBRUSH)SelectObject(hdc, hbrush);

		switch (s.shape_sel) {
		case 2:
			MoveToEx(hdc, rect.left, rect.top, NULL);
			LineTo(hdc, rect.right, rect.bottom);
			break;
		case 3:
		{
			POINT p[6]{
				{rect.right / 2 + rect.left / 2., rect.top},
				{rect.left, rect.bottom},
				{rect.right, rect.bottom} };
			Polygon(hdc, p, 3);
			break;
		}
		case 4:
		{
			POINT p[8]{
				{rect.left, rect.top},
				{rect.left, rect.bottom},
				{rect.right, rect.bottom},
				{rect.right, rect.top} };
			Polygon(hdc, p, 4);
			break;
		}
		case 5:
		{
			POINT p[10]{
				{(rect.left + rect.right) / 2., rect.top},
				{rect.right ,(rect.top + rect.bottom) / 2. - (rect.right + rect.left) / 10},
				{rect.right - (rect.right + rect.left) / 3, rect.bottom},
				{rect.left + (rect.right + rect.left) / 3, rect.bottom},
				{rect.left ,(rect.top + rect.bottom) / 2. - (rect.right + rect.left) / 10} };
			Polygon(hdc, p, 5);
			break;
		}
		case 6:
		{
			POINT p[12]{
				{ (rect.left + rect.right) / 2., rect.top },
				{ rect.left, (rect.top + rect.bottom) / 2. - (rect.top + rect.bottom) / 4.},
				{ rect.left, (rect.top + rect.bottom) / 2. + (rect.top + rect.bottom) / 4. },
				{ (rect.left + rect.right) / 2., rect.bottom },
				{ rect.right, (rect.top + rect.bottom) / 2. + (rect.top + rect.bottom) / 4. },
				{ rect.right, (rect.top + rect.bottom) / 2. - (rect.top + rect.bottom) / 4. }
			};
			break;
		}
		}

		SelectObject(hdc, old_pen);
		SelectObject(hdc, old_brush);
		DeleteObject(hpen);
		DeleteObject(hbrush);
	}


}