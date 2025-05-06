#include "head.h"

#define marginX 16
#define marginY 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2025 Window Programming Exam";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int  WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR lpszCmdParam, _In_  int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	//--- 크기 변경 가능 (기존 (1024, 768))
	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		1600,
		900,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mdc;
	PAINTSTRUCT ps;
	HBITMAP hbitmap;
	TCHAR txt[100]{};

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	static RECT clientRect;
	static RECT f_rect;
	static int rect_width;
	static int rect_height;

	static int startx{}, starty{};
	static int oldx{}, oldy{};
	static int endx{}, endy{};

	static Cell board[ROW][COL]{};
	COLORREF color[]{RGB(230, 60, 60), RGB(63, 73, 204), RGB(251, 237, 68), RGB(54, 224, 18), RGB(26, 24, 21), RGB(226, 239, 239)};
	COLORREF invert_color[] = {
	RGB(25, 195, 195),
	RGB(192, 182, 51),
	RGB(4, 18, 187),
	RGB(201, 31, 237),
	RGB(229, 231, 234),
	RGB(29, 16, 16)
	};

	static vector<Rect> shapes;

	static int Grid_style{};
	static int Color_style{1};
	static bool Border_flag{};
	static bool Move_flag{};
	static bool Inform_flag{};

	static bool LB_Drag{};

	static int select_index{-1};
	static int move_rect_index{-1};
	 
	switch (iMessage) {
	case WM_COMMAND:
		switch (wParam) {
		case ID_GRID_SOLID:
		{
			Grid_style = 0;
			break;
		}
		case ID_GRID_DOT:
		{
			Grid_style = 1;
			break;
		}
		case ID_GRID_DASH:
		{
			Grid_style = 2;
			break;
		}
		case ID_COLOR_RED:
		{
			Color_style = 1;
			break;
		}
		case ID_COLOR_BLUE:
		{
			Color_style = 2;
			break;
		}
		case ID_COLOR_YELLOW:
		{
			Color_style = 3;
			break;
		}
		case ID_COLOR_GREEN:
		{
			Color_style = 4;
			break;
		}
		case ID_COLOR_BLACK:
		{
			Color_style = 5;
			break;
		}
		case ID_COLOR_WHITE:
		{
			Color_style = 6;
			break;
		}
		case ID_BORDER_ON:
		{
			Border_flag = true;
			break;
		}
		case ID_BORDER_OFF:
		{
			Border_flag = false;
			break;
		}
		case ID_MOVE_ON:
		{
			Move_flag = true;
			break;
		}
		case ID_MOVE_OFF:
		{
			Move_flag = false;
			break;
		}
		case ID_INFORM_ON:
		{
			Inform_flag = true;
			break;
		}
		case ID_INFORM_OFF:
		{
			Inform_flag = false;
			break;
		}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CREATE:
		GetClientRect(hWnd, &clientRect);
		f_rect = {clientRect.left, clientRect.top, clientRect.right-500, clientRect.bottom};

		rect_width={ (int)(f_rect.right - f_rect.left) / COL };
		rect_height={ (int)(f_rect.bottom - f_rect.top) / ROW };

		break;
	case WM_CHAR:
	{
		switch (wParam) {
		case '1':
			select_index = 0;
			break;
		case '2':
			select_index = 1;
			break;
		case '3':
			select_index = 2;
			break;
		case '4':
			select_index = 3;
			break;
		case '5':
			select_index = 4;
			break;
		}
		break;
	}
	case WM_KEYDOWN:
	{
		if(shapes.size()>0 and Move_flag){
			Rect& r = shapes.at(select_index);

			switch (wParam) {
			case VK_LEFT:
				if (r.sx > 0) {
					r.sx--;
					r.ex--;
				}
				break;
			case VK_RIGHT:
				if (r.ex < COL) {
					r.sx++;
					r.ex++;
				}
				break;
			case VK_UP:
				if (r.sy > 0) {
					r.sy--;
					r.ey--;
				}
				break;
			case VK_DOWN:
				if (r.ey < ROW) {
					r.sy++;
					r.ey++;
				}
				break;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		LB_Drag = TRUE;

		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		POINT mp{ mx, my };

		if (PtInRect(&f_rect, mp)) {
			startx = oldx = mx;
			starty = oldy = my;
		}

		break;
	}
	case WM_LBUTTONUP:
	{
		hdc = GetDC(hWnd);

		LB_Drag = FALSE;

		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		endx = mx;
		endy = my;

		POINT mp{ mx, my };

		if (PtInRect(&f_rect, mp)) {
			if (Border_flag) {
				Rectangle(hdc, startx, starty, oldx, oldy);
			}
			else {
				RECT oldrect{ startx, starty, oldx, oldy };
				FillRect(hdc, &oldrect, hbrush);
			}

			if (shapes.size() < 5) {
				int sx{ (startx - f_rect.left) / rect_width };
				int sy{ (starty - f_rect.top) / rect_height };
				int ex{ (endx - f_rect.left) / rect_width + 1 };
				int ey{ (endy - f_rect.top) / rect_height + 1 };

				shapes.emplace_back(sx, sy, ex, ey, Color_style, Border_flag);
				select_index = shapes.size() - 1;
			}
		}
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hWnd);

		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		POINT mp{ mx, my };

		if (PtInRect(&f_rect, mp)) {
			if (LB_Drag) {
				if (Color_style > 0)
					hbrush = CreateSolidBrush(color[Color_style - 1]);

				SetROP2(hdc, R2_NOTXORPEN);
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
				if (Color_style > 0)
					oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
				else
					SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));

				endx = mx;
				endy = my;

				wsprintf(txt, TEXT("startx/y: %d %d"), startx, starty);
				TextOut(hdc, clientRect.right - 490, clientRect.top + 10, txt, lstrlen(txt));
				wsprintf(txt, TEXT("oldx/y: %d %d"), oldx, oldy);
				TextOut(hdc, clientRect.right - 490, clientRect.top + 40, txt, lstrlen(txt));
				wsprintf(txt, TEXT("endx/y: %d %d"), endx, endy);
				TextOut(hdc, clientRect.right - 490, clientRect.top + 70, txt, lstrlen(txt));

				if (Border_flag) {
					hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
					oldpen = (HPEN)SelectObject(hdc, hpen);

					Rectangle(hdc, startx, starty, oldx, oldy);
					Rectangle(hdc, startx, starty, endx, endy);

					SelectObject(hdc, oldpen);
					DeleteObject(hpen);
				}
				else {
					HBRUSH hbrush2{};
					if (Color_style > 0) {
						hbrush2 = CreateSolidBrush(invert_color[Color_style - 1]);
						SelectObject(hdc, hbrush2);
					}
					else
						SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));

					PatBlt(hdc, startx, starty, oldx - startx, oldy - starty, PATINVERT);
					PatBlt(hdc, startx, starty, endx - startx, endy - starty, PATINVERT);

					if (Color_style > 0) {
						SelectObject(hdc, hbrush);
						DeleteObject(hbrush2);
					}
				}
				oldx = endx; oldy = endy;

				if (Color_style) {
					SelectObject(hdc, oldbrush);
					DeleteObject(hbrush);
				}
			}
		}
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		POINT mp{ mx, my };

		for (int i{}; i < shapes.size(); i++) {
			Rect& r = shapes.at(i);

			// 시작 지점 (왼쪽 위 좌표)
			int left = f_rect.left + rect_width * r.sx;
			int top = f_rect.top + rect_height * r.sy;
			// 끝 지점 (오른쪽 아래 좌표)
			int right = f_rect.left + rect_width * r.ex;
			int bottom = f_rect.top + rect_height * r.ey;

			RECT rect = { left, top, right, bottom };

			if (PtInRect(&rect, mp)) {
				startx = mx;
				starty = my;
				move_rect_index = i;
			}
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		endx = mx;
		endy = my;

		int dx = (endx - startx) / rect_width;
		int dy = (endy - starty) / rect_height;

		if(move_rect_index>-1){
			Rect& r = shapes.at(move_rect_index);
			r.sx += dx;
			r.ex += dx;
			r.sy += dy;
			r.ey += dy;
			move_rect_index = -1;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		int mx{ LOWORD(lParam) };
		int my{ HIWORD(lParam) };

		POINT mp{ mx, my };

		for (int i{}; i < shapes.size(); i++) {
			Rect& r = shapes.at(i);

			// 시작 지점 (왼쪽 위 좌표)
			int left = f_rect.left + rect_width * r.sx;
			int top = f_rect.top + rect_height * r.sy;

			// 끝 지점 (오른쪽 아래 좌표)
			int right = f_rect.left + rect_width * r.ex;
			int bottom = f_rect.top + rect_height * r.ey;

			RECT rect = { left, top, right, bottom };

			if (PtInRect(&rect, mp)) {
				r.DB_click_flag = r.DB_click_flag ? false : true;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		f_rect = { clientRect.left, clientRect.top, clientRect.right - 500, clientRect.bottom };


		GRID_PAINT(mdc, f_rect, board,
			Grid_style, shapes, select_index);

		if (Inform_flag) {
			wsprintf(txt, TEXT("도형의 개수: %d"), shapes.size());
			TextOut(mdc, clientRect.right - 490, clientRect.top + 10, txt, lstrlen(txt));
			for (int i{}; i < shapes.size(); i++) {
				Rect& r = shapes.at(i);
				int width = r.ex - r.sx;
				int height = r.ey - r.sy;

				wsprintf(txt, TEXT("%d번째 도형 - 가로: %d, 세로: %d, 색: %d"), i+1, width, height, r.color);
				TextOut(mdc, clientRect.right - 490, clientRect.top + 10 + 30 * (i + 1), txt, lstrlen(txt));
				wsprintf(txt, TEXT("%d %d %d %d"), r.sx, r.sy, r.ex, r.ey);
				TextOut(mdc, clientRect.right - 490, clientRect.top + 40 + 30 * (i + 1), txt, lstrlen(txt));

				TextOut(mdc, clientRect.right - 490, clientRect.top + 400, TEXT("1-빨강 2-파랑 3-노랑 4-초록 5-검정 6-하양"), lstrlen(TEXT("1-빨강 2-파랑 3-노랑 4-초록 5-검정 6-하양")));

			}
		}

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hbitmap);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
