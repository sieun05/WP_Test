#pragma once
#include <windows.h>

void Circle_Paint(HDC hdc, const RECT& rect);

void Circle_Paint(HDC hdc, const RECT& rect) {
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}