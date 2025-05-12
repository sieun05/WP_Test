#include "head.h"

#define marginX 16
#define marginY 39

std::random_device rd; 
std::mt19937 gen(rd()); 
std::uniform_int_distribution<> width(0, 1200);
std::uniform_int_distribution<> spd(3, 10); 

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
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	//--- 크기 변경 가능 (기존 (1024, 768))
	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		1280,
		800,
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
	HDC hdc, mdc, memdc;
	PAINTSTRUCT ps;
	HBITMAP hbitmap;
	static RECT clientRect, f_rect{};

	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	static HBITMAP image;
	static HBITMAP character[6];

	static int startx, starty;
	static int endx, endy;
	static int oldx, oldy;

	static bool LB_Drag{ false };

	static int c_delay{};
	static int c_num{};
	static Character char_arr[30]{};

	static bool rect_flag{};

	TCHAR txt[100]{};

	switch (iMessage) {
	case WM_CREATE:
		

		GetClientRect(hWnd, &clientRect);

		image = (HBITMAP)LoadImage(g_hInst, TEXT("images\\unnamed.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[2] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[3] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[4] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch5.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[5] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\ch6.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		for (int i{0}; i < 30; i++) {
			Character& c = char_arr[i];

			c.x = width(gen);
			c.y = 0;
			c.speed = spd(gen);
		}

		break;
	case WM_TIMER:
		c_delay++;
		if (c_delay > 2) {
			c_delay = 0;
			c_num++;
			if (c_num > 5)
				c_num %= 6;
		}

		for (int i{}; i < 30; i++) {
			Character& c = char_arr[i];

			c.y += c.speed;
			if (c.y + 90 > clientRect.bottom)
				c.y = 0;

			if (rect_flag and c.x > f_rect.left and c.x + 40 < f_rect.right and c.y + 90 > f_rect.bottom) {
				c.y -= c.speed;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CHAR:
		switch (wParam) {
		case 'p':
			SetTimer(hWnd, 1, 50, NULL);
			break;
		case 'd':
			rect_flag = false;
			break;
		case 'r':
			for (int i{ 0 }; i < 30; i++) {
				Character& c = char_arr[i];

				c.y = 0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
		case WM_LBUTTONDOWN:
		{

			LB_Drag = TRUE;

			int mx{ LOWORD(lParam) };
			int my{ HIWORD(lParam) };

			startx = oldx = mx;
			starty = oldy = my;

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

			f_rect={ startx, starty, oldx, oldy };
			rect_flag = true;
			
			ReleaseDC(hWnd, hdc);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if(LB_Drag){
				hdc = GetDC(hWnd);

				int mx{ LOWORD(lParam) };
				int my{ HIWORD(lParam) };

				SetROP2(hdc, R2_XORPEN);
				SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

				endx = mx;
				endy = my;

				Rectangle(hdc, startx, starty, oldx, oldy);
				Rectangle(hdc, startx, starty, endx, endy);

				oldx = endx; oldy = endy;

				ReleaseDC(hWnd, hdc);
			}
			break;
		}
	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);

		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		memdc = CreateCompatibleDC(mdc);

		SelectObject(memdc, image);
		StretchBlt(mdc, 0, 0, clientRect.right, clientRect.bottom, memdc, 0, 0, 1024, 1024, SRCCOPY);

		SelectObject(memdc, character[c_num]);
		for (int i{}; i < 30; i++) {
			const Character& c = char_arr[i];

			TransparentBlt(mdc, c.x, c.y, 90, 90, memdc, 0, 0, 90, 90, RGB(0, 0, 64));
		}
		
		if (rect_flag) {
			FrameRect(mdc, &f_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);
		DeleteObject(hbitmap);
		DeleteDC(memdc);
		DeleteDC(mdc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
