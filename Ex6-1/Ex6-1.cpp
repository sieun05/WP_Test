#include "Ex6-1.h"

#define marginX 16
#define marginY 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2025 Window Programming Exam";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dialog_Proc(HWND, UINT, WPARAM, LPARAM);


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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1024 + marginX, 768 + marginY, NULL, (HMENU)NULL, hInstance, NULL);
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

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)Dialog_Proc);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK Dialog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, mdc;
	PAINTSTRUCT ps;
	HBITMAP hbitmap;
	static RECT clientRect;

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	static int check[2]{};
	static int radio[2]{};

	int mx, my;
	static int startx, starty;
	static int endx, endy;
	static int oldx, oldy;

	static bool LB_Drag{};

	static RECT rect{ 50, 50, 100, 100 };

	static bool check_draw{};
	static bool check_move{};

	switch (iMsg) {
	case WM_INITDIALOG:


		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			check_draw = true;
			InvalidateRect(hDlg, NULL, FALSE);
			break;
		case IDC_BUTTON2:
			check_move = true;
			break;
		case IDC_BUTTON3:
			EndDialog(hDlg, 0);
			break;
		case IDC_RADIO1:
			radio[0] = 0;
			break;
		case IDC_RADIO2:
			radio[0] = 1;
			break;
		case IDC_RADIO3:
			radio[0] = 2;
			break;
		case IDC_RADIO4:
			radio[1] = 0;
			break;
		case IDC_RADIO5:
			radio[1] = 1;
			break;
		case IDC_CHECK1:
			check[0] = 1 - check[0];
			break;
		case IDC_CHECK2:
			check[1] = 1 - check[1];
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{

		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		POINT mp{ mx, my };
		if (PtInRect(&rect, mp)) {
			oldx = startx = (rect.left + rect.right) / 2;
			oldy = starty = (rect.top + rect.bottom) / 2;

			LB_Drag = true;
		}
		//InvalidateRect(hDlg, NULL, FALSE);
		break;
	}
	case WM_LBUTTONUP:
	{
		LB_Drag = false;
		endx = LOWORD(lParam);
		endy = HIWORD(lParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		hdc = GetDC(hDlg);

		if (LB_Drag and check_draw) {
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			endx = LOWORD(lParam);
			endy = HIWORD(lParam);

			MoveToEx(hdc, startx, starty, NULL);
			LineTo(hdc, oldx, oldy);

			MoveToEx(hdc, startx, starty, NULL);
			LineTo(hdc, endx, endy);

			oldx = endx;
			oldy = endy;
		}

		ReleaseDC(hDlg, hdc);
		break;
	}
	case WM_PAINT:
	{
		GetClientRect(hDlg, &clientRect);
		hdc = BeginPaint(hDlg, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		Rectangle(mdc, rect.left, rect.top, rect.right, rect.bottom);

		/*if (not LB_Drag) {
			MoveToEx(mdc, startx, starty, NULL);
			LineTo(mdc, endx, endy);
		}*/

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteObject(hbitmap);
		DeleteDC(mdc);
		EndPaint(hDlg, &ps);
		break;
	}
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}

	return 0;
}