// main.cpp
#include "head.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR frm_title = TEXT("c_form_view");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Msg_frm;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = frm_title;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		frm_title,
		TEXT("test"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 800,
		NULL, (HMENU)NULL,
		hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Msg_frm, NULL, 0, 0)) {
		TranslateMessage(&Msg_frm);
		DispatchMessage(&Msg_frm);
	}
	return (int)Msg_frm.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	static RECT clientRect;
	static Cell board[ROW][COLUMN]{};
	static std::vector<Circle> shapes;
	static int fps{ 300 };
	static int obstacle_num{};

	switch (iMsg) {
	case WM_CREATE:
		shapes.emplace_back(0, 0, 1, 0, color(gen));
		break;

	case WM_TIMER:
		Circle_Move(shapes, board); // <-- Crash_Check 포함됨
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &clientRect);

		// PAINT 대신 직접 구현
		COLORREF color[]{ RGB(247, 213, 156), RGB(154, 219, 138), RGB(134, 192, 226), RGB(235, 117, 132), RGB(132, 112, 238), RGB(226, 153, 230) };
		float rect_width{ (float)(clientRect.right - clientRect.left) / COLUMN };
		float rect_height{ (float)(clientRect.bottom - clientRect.top) / ROW };

		for (int i = 0; i < ROW; ++i) {
			for (int j = 0; j < COLUMN; ++j) {
				RECT rect{ clientRect.left, clientRect.top, clientRect.left + rect_width, clientRect.top + rect_height };
				OffsetRect(&rect, rect_width * j, rect_height * i);
				if (board[i][j].type == 1) {
					HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
					FillRect(hdc, &rect, redBrush);
					DeleteObject(redBrush);
				}
				else if (board[i][j].type == 2) {
					InflateRect(&rect, -5, -5);
					HBRUSH hBrush = CreateSolidBrush(color[board[i][j].color]);
					FillRect(hdc, &rect, hBrush);
					DeleteObject(hBrush);
				}
			}
		}

		for (const auto& s : shapes) {
			RECT rect{ clientRect.left, clientRect.top, clientRect.left + rect_width, clientRect.top + rect_height };
			OffsetRect(&rect, rect_width * s.x, rect_height * s.y);

			// 크기 조절: 꼬리는 점점 작게
			int shrink = 0;
			int parent = s.parent_index;
			while (parent != -1) {
				++shrink;
				parent = shapes[parent].parent_index;
			}
			InflateRect(&rect, -shrink * 1, -shrink * 1);

			HBRUSH hBrush = CreateSolidBrush(color[s.color]);
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_CHAR:
		if (wParam == 'a') {
			AddTail(shapes);
		}
		else if (wParam == 's') {
			SetTimer(hWnd, 1, fps, NULL);
		}
		else if (wParam == 'q') {
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}