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
	HBITMAP hbitmap;
	static RECT clientRect;
	TCHAR txt[100];

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	static HBITMAP image[2];

	static int sel_image{};

	static bool LB_Drag{};
	static int startx, starty, oldx, oldy, endx, endy;
	static RECT f_rect;
	static bool rect_flag{};
	static bool rect_view_flag{};

	static int image_size{};

	static bool f_key_flag{};
	static bool invert_image_flag{};
	static bool reset_flag{};

	static bool h_key_flag{};
	static bool v_key_flag{};

	static bool m_key_flag{};
	static int m_dx{ 5 };
	static int m_dy{ 5 };

	static bool n_key_flag{};
	static bool size_up_flag{};
	static int n_image_size{};

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 50, NULL);
		GetClientRect(hWnd, &clientRect);

		image[0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\image2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		image[1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\image3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		break;
	case WM_TIMER:
	{
		switch (wParam) {
		case 1:
			if (reset_flag) {
				reset_flag = false;

				rect_flag = false;
				rect_view_flag = false;
				image_size = 0;
				f_key_flag = false;
				invert_image_flag = false;

				h_key_flag = false;
				v_key_flag = false;

				m_key_flag = false;
				m_dx={ 5 };
				m_dy={ 5 };

				n_key_flag = false;
				size_up_flag = false;
				n_image_size = 0;
			}

			if (n_key_flag) {
				if (size_up_flag) {
					n_image_size+=4;
					if (n_image_size > 100)
						size_up_flag = false;
				}
				else {
					n_image_size-=4;
					if(n_image_size < -100)
						size_up_flag = true;
				}
			}

			if (m_key_flag) {
				OffsetRect(&f_rect, m_dx, m_dy);

				if (f_rect.left < 0 or f_rect.right > 1280) {
					m_dx *= -1;
					OffsetRect(&f_rect, m_dx, 0);
				}
				if (f_rect.top < 0 or f_rect.bottom > 720) {
					m_dy *= -1;
					OffsetRect(&f_rect, 0, m_dy);
				}
			}

			int dx{}, dy{};

			if (GetAsyncKeyState(VK_LEFT) & 0x8000) dx = -5;
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) dx = 5;
			if (GetAsyncKeyState(VK_UP) & 0x8000) dy = -5;
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) dy = 5;

			OffsetRect(&f_rect, dx, dy);

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;
	}
	case WM_CHAR:
		switch (wParam) {
		case '1':
			reset_flag = true;
			sel_image = 0;
			break;
		case '2':
			reset_flag = true;
			sel_image = 1;
			break;
		case 'e':
			image_size--;
			if (image_size < -50)
				image_size = 0;
			break;
		case 's':
			image_size++;
			if (image_size > 50)
				image_size = 0;
			break;
		case 'b':
			image_size = 0;
			break;
		case 'f':
			f_key_flag = f_key_flag ? false : true;
			break;
		case 'r':
			reset_flag = true;
			break;
		case 'i':
			invert_image_flag = invert_image_flag ? false : true;
			break;
		case 'h':
			h_key_flag = h_key_flag ? false : true;
			break;
		case 'v':
			v_key_flag = v_key_flag ? false : true;
			break;
		case 'm':
			m_key_flag = m_key_flag ? false : true;
			break;
		case 'n':
			n_key_flag = n_key_flag ? false : true;
			break;
		case 'p':
			rect_view_flag = true;
			break;
		case 'q':
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;
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

		if(not rect_flag){

			int mx{ LOWORD(lParam) };
			int my{ HIWORD(lParam) };

			endx = mx;
			endy = my;

			f_rect = { startx, starty, oldx, oldy };
			rect_flag = true;
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

		POINT mp{ mx,my };

		if (LB_Drag and not rect_flag) {

			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
			hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);     // 내부 투명
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);

			endx = mx;
			endy = my;

			Rectangle(hdc, startx, starty, oldx, oldy);
			Rectangle(hdc, startx, starty, endx, endy);

			oldx = endx; oldy = endy;

			SelectObject(hdc, oldbrush);
		}

		if (LB_Drag and rect_flag) {
			int dx = mx - oldx;
			int dy = my - oldy;

			if(PtInRect(&f_rect, mp))
				OffsetRect(&f_rect, dx, dy);

			POINT rp[4]{
				{f_rect.left, f_rect.top },
				{f_rect.left, f_rect.bottom},
				{f_rect.right, f_rect.top},
				{f_rect.right, f_rect.bottom}
			};

			bool corner_flag{ false };
			int sel_p{};

			for (int i{}; i < 4; i++) {
				if (mp.x <= rp[i].x +5 and mp.x >= rp[i].x - 5 and mp.y <= rp[i].y + 5 and mp.y >= rp[i].y - 5) {
					corner_flag = true;
					sel_p = i;
					break;
				}
			}
			
			if (corner_flag) {
				switch (sel_p) {
				case 0:
					f_rect.left += dx;
					f_rect.top += dy;
					break;
				case 1:
					f_rect.left += dx;
					f_rect.bottom += dy;
					break;
				case 2:
					f_rect.right += dx;
					f_rect.top += dy;
					break;
				case 3:
					f_rect.right += dx;
					f_rect.bottom += dy;
					break;
				}
			}

			oldx = mx;
			oldy = my;

			InvalidateRect(hWnd, NULL, FALSE);
		}
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		memdc = CreateCompatibleDC(mdc);

		SelectObject(memdc, image[sel_image]);
		StretchBlt(mdc, 0, 0, clientRect.right, clientRect.bottom, memdc, 0, 0, 1280, 720, SRCCOPY);

		if (rect_flag and not n_key_flag) {
			FrameRect(mdc, &f_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}

		if (rect_view_flag) {
			RECT temp_rect{ f_rect };
			InflateRect(&temp_rect, image_size, image_size);
			if (h_key_flag) {
				temp_rect = { temp_rect.right, temp_rect.top, temp_rect.left, temp_rect.bottom };
			}
			if (v_key_flag) {
				temp_rect = { temp_rect.left, temp_rect.bottom, temp_rect.right, temp_rect.top };
			}
			
			int property{};
			if (invert_image_flag) property = NOTSRCCOPY;
			else  property = SRCCOPY;

			if (f_key_flag) {
				StretchBlt(mdc, 0, 0, clientRect.right, clientRect.bottom,
					memdc, temp_rect.left, temp_rect.top, temp_rect.right - temp_rect.left, temp_rect.bottom - temp_rect.top, property);
			}
			else if (n_key_flag) {
				RECT n_rect{ f_rect };
				InflateRect(&n_rect, n_image_size, n_image_size);
				temp_rect={ n_rect };
				InflateRect(&temp_rect, image_size, image_size);

				if (h_key_flag) {
					temp_rect = { temp_rect.right, temp_rect.top, temp_rect.left, temp_rect.bottom };
				}
				if (v_key_flag) {
					temp_rect = { temp_rect.left, temp_rect.bottom, temp_rect.right, temp_rect.top };
				}

				StretchBlt(mdc, 0, 0, n_rect.right - n_rect.left, n_rect.bottom - n_rect.top,
					memdc, temp_rect.left, temp_rect.top, temp_rect.right - temp_rect.left, temp_rect.bottom - temp_rect.top, property);

				FrameRect(mdc, &n_rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
			}
			else {
				StretchBlt(mdc, 0, 0, f_rect.right - f_rect.left, f_rect.bottom - f_rect.top,
					memdc, temp_rect.left, temp_rect.top, temp_rect.right - temp_rect.left, temp_rect.bottom - temp_rect.top, property);
			}
		}

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(memdc);
		DeleteObject(hbitmap);
		DeleteDC(mdc);
		EndPaint(hWnd, &ps);
		break;
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
