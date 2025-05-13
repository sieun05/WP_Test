#include "head.h"

#define marginX 16
#define marginY 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Agario";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	//--- 크기 변경 가능 (기존 (1024, 768))
	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		1280 + marginX,
		720 + marginY,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL
	);
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
	HBITMAP hBitmap;
	static RECT clientRect;
	static int centerX, centerY;

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};
	TCHAR txt[100];

	static int screen{}; //0 메인화면, 1 게임플레이, 2 게임오버
	
	static int camera_mode{ 0 }; //0 이동모드 1 고정모드
	static bool pause_flag{ false };
	static bool display_flag{ false }; //먹은 먹이 수, 플레이 시간, 세포의 크기
	static bool reset_flag{ false };

	static vector<Cell> cell;

	static int mx, my;

	switch (iMessage) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 50, NULL);
		GetClientRect(hWnd, &clientRect);
		centerX = (clientRect.left + clientRect.right) / 2;
		centerY = (clientRect.top + clientRect.bottom) / 2;

		int cell_size{ 20 };
		int cell_x{ centerX };
		int cell_y{ centerY };
		cell.emplace_back(cell_x, cell_y, cell_size, (float)70 / cell_size);

		break;
	}
	case WM_TIMER:
	{

		Cell_Move(cell, mx, my, clientRect, camera_mode);

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_MOUSEMOVE:
	{
		mx={ LOWORD(lParam) };
		my={ HIWORD(lParam) };

		break;
	}
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hBitmap);

		hbrush = CreateSolidBrush(RGB(233, 245, 245));
		FillRect(mdc, &clientRect, hbrush);
		DeleteObject(hbrush);
		
		GRID_PAINT(mdc, clientRect, cell);

		if (camera_mode == 1)
			BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);
		else {
			// 세포 크기가 커질수록 더 많이 보이게 하기 위해 줌 계수를 설정
			double zoom_factor{ 0.008 * cell.at(0).size };
			if (zoom_factor > 1.0) zoom_factor = 1.0;
			if (zoom_factor < 0.3) zoom_factor = 0.3;

			// 보일 실제 맵 영역 크기 계산 (줌 아웃 효과)
			int map_width = (double)(clientRect.right * zoom_factor);
			int map_height = (double)(clientRect.bottom * zoom_factor);

			// 중심 세포 좌표 기준으로 영역 계산
			int src_left = cell.at(0).x - map_width / 2;
			int src_top = cell.at(0).y - map_height / 2;

			// Clamp 영역 (음수 또는 맵 바깥으로 못 나가게)
			if (src_left < 0) src_left = 0;
			if (src_top < 0) src_top = 0;
			if (src_left + map_width > 1280 ) src_left =1280 - map_width;
			if (src_top + map_height > 720) src_top = 720 - map_height;

			// 출력
			StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
				mdc, src_left, src_top, map_width, map_height, SRCCOPY);

			//StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, cell.at(0).x - width/2, cell.at(0).y - height/2, width, height, SRCCOPY);
		}

		wsprintf(txt, TEXT("mouse: %d %d | cell: %d %d"), mx, my, cell.at(0).x, cell.at(0).y);
		TextOut(hdc, clientRect.right - 300, 10, txt, lstrlen(txt));

		DeleteObject(hBitmap);
		DeleteDC(mdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		switch (wParam) {
		case ID_GAME_START:
		{
			if (screen == 0) {
				reset_flag = true;
				screen = 1;
			}
			break;
		}
		case ID_GAME_PAUSE:
		{
			pause_flag = pause_flag ? false : true;
			break;
		}
		case ID_GAME_RESET:
		{
			reset_flag = true;
			break;
		}
		case ID_GAME_EXIT:
		{
			PostQuitMessage(0);
			return 0;
			break;
		}
		case ID_VIEW_FOLLOWPLAYER:
		{
			camera_mode = 0;
			break;
		}
		case ID_VIEW_FIXEDMAP:
		{
			camera_mode = 1;
			break;
		}
		case ID_HUDINFODISPLAY_ON:
		{
			display_flag = true;
			break;
		}
		case ID_HUDINFODISPLAY_OFF:
		{
			display_flag = false;
			break;
		}
		}
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
