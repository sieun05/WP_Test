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
		800,
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
	static HBITMAP image;
	static RECT clientRect, f_rect{};

	HBRUSH hbrush, oldbrush;
	HPEN hpen, oldpen;

	static int size{};

	static bool keyA_flag{ true };
	static bool keyR_flag{ false };
	static int devide{ 1 };
	static int sel_idx{ -1 };

	TCHAR txt[100]{};

	switch (iMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &clientRect);
		f_rect = clientRect;

		image = (HBITMAP)LoadImage(g_hInst, TEXT("images\\unnamed.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (image == NULL) {
			MessageBox(hWnd, TEXT("이미지 로드 실패"), TEXT("오류"), MB_OK);
		}

		break;
	case WM_CHAR:
		switch (wParam) {
		case 'a':
			keyA_flag = keyA_flag ? false : true;
			break;
		case '+':
			if (size < 0)
				size++;
			break;
		case '-':
			size--;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
			devide = wParam - '0';
			break;
		case 'r':
			keyR_flag = keyR_flag ? false : true;
			break;
		case 'q':
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		int length{ (f_rect.right - f_rect.left) / devide };

		for (int i = 0; i < devide; i++) {
			int left = f_rect.left + length * i;
			int right = f_rect.left + length * (i + 1);
			if (mx >= left && mx < right) {
				sel_idx = i;
				break;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		f_rect = clientRect;
		InflateRect(&f_rect, size, size);

		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		memdc = CreateCompatibleDC(mdc);

		SelectObject(memdc, image);
		if (keyA_flag) {
			StretchBlt(mdc, 0, 0, clientRect.right, clientRect.bottom, memdc, 0, 0, 1024, 1024, SRCCOPY);
		}
		else {
			int length{ (f_rect.right - f_rect.left) / devide };

			for (int i{}; i < devide; i++) {
				

				wsprintf(txt, TEXT("sel_idx: %d"), sel_idx);
				TextOut(mdc, 10, 10, txt, lstrlen(txt));

				if (sel_idx == i) {
					if(keyR_flag)
						StretchBlt(mdc, f_rect.left + (length * i), f_rect.top, length, f_rect.bottom - f_rect.top, memdc, 0, 0, 1024, 1024, NOTSRCCOPY);
					else
						StretchBlt(mdc, f_rect.left + (length * i), f_rect.top, length, f_rect.bottom - f_rect.top, memdc, 0, 0, 1024, 1024, SRCCOPY);

					RECT rect{ f_rect.left + (length * i), f_rect.top, f_rect.left + length * (i + 1), f_rect.bottom };
					hbrush = CreateSolidBrush(RGB(255, 0, 0));
					FrameRect(mdc, &rect, hbrush);
					DeleteObject(hbrush);
				}
				else {
					StretchBlt(mdc, f_rect.left + (length * i), f_rect.top, length, f_rect.bottom - f_rect.top, memdc, 0, 0, 1024, 1024, SRCCOPY);
				}
			}
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
