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
	BITMAP bit1;
	static int Bw{}, Bh{};

	HBRUSH hbrush{}, oldbrush{};
	HPEN hpen{}, oldpen{};

	//배경 관련 변수
	static HBITMAP back[3];
	static int back_x[3]{};

	//캐릭터 관련 변수
	static HBITMAP character[4];
	int char_order[6]{0, 1, 2, 3, 2, 1};
	static int char_img_num{};
	static int char_img_delay{};
	static int char_x{};
	static int char_y{ 560 };

	//캐릭터 행동 관련 변수
	static int char_jump_y{};
	static bool char_jump_flag{};
	static int jump_v = 0.0;
	const float gravity = 1.2;
	const float initial_jump_v = -22.0;

	static bool char_slide_flag{};

	static HBITMAP crash_image[2];
	static int crash_img_delay{};
	static int crash_img_num{};
	static bool crash_flag{};
	static int crash_delay{};
	static RECT crash_rect;

	static HBITMAP monster[2][7];
	static int monster_x{ -90 };
	static int monster_y{ 550 };
	static int sel_monster{};
	static int monster_img_num{};
	static int monster_img_delay{};

	static bool game_start_flag{ true };
	static int game_start_delay{};

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 25, NULL);
		GetClientRect(hWnd, &clientRect);

		back[0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\backimage1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		back[1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\backimage2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		back[2] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\backimage3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		character[0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dog1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dog2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[2] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dog3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		character[3] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dog4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		crash_image[0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\crashimage.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		crash_image[1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\crashimage2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		monster[0][0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][2] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][3] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][4] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d5.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][5] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d6.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[0][6] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\d7.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		monster[1][0] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][1] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][2] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd3.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][3] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd4.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][4] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd5.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][5] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd6.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		monster[1][6] = (HBITMAP)LoadImage(g_hInst, TEXT("images\\dd7.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		char_x = clientRect.right - 45;

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:

			//캐릭터 이동
			if (game_start_flag) {
				char_x -= 8;
				if (char_x < clientRect.right - 250)
					game_start_flag = false;
			}
			else {
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					char_x -= 5;
					if (char_x < -45) char_x = -45;
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					char_x += 5;
					if (char_x + 90 > clientRect.right + 45) char_x = clientRect.right - 45;
				}
			}

			//캐릭터 애니메이션
			char_img_delay++;
			if (char_img_delay > 6) {
				char_img_delay = 0;
				char_img_num++;
				if (char_img_num >= 4) char_img_num = 0;
			}

			//캐릭터 점프
			if (char_jump_flag) {	
				char_jump_y += (int)jump_v;
				jump_v += gravity;

				if (char_jump_y >= 0) {
					char_jump_flag = false;
					char_jump_y = 0;
					jump_v = 0.0;
				}
			}

			//캐릭터 슬라이드
			if (not char_jump_flag and GetKeyState('K') & 0x8000) {
				char_slide_flag = true;
			}
			else {
				char_slide_flag = false;
			}


			//몬스터 이동
			monster_x += 9;
			if (monster_x > clientRect.right) {
				sel_monster = r_flag(gen);
				monster_y = 550 - 70 * r_flag(gen);
				monster_x = -90;
			}

			//몬스터 애니메이션
			monster_img_delay++;
			if (monster_img_delay > 5) {
				monster_img_delay = 0;
				monster_img_num++;
				if (monster_img_num >= 7) monster_img_num = 0;
			}

			//충돌
			RECT mon_rect{ monster_x + 16, monster_y + 8, monster_x + 16 + 55, monster_y + 90 };
			RECT char_rect{};
			if(char_jump_flag)
				char_rect = { char_x + 7, char_y + char_jump_y + 7, char_x + 7 + 60, char_y + char_jump_y + 66 - 4 };
			else if(char_slide_flag)
				char_rect = { char_x + 7, char_y + 33 + 7/2 / 2, char_x + 7 + 60, char_y + 66 - 2 };
			else
				char_rect = { char_x + 7, char_y + 7, char_x + 7 + 60, char_y + 66 - 4 };


			if (IntersectRect(&crash_rect, &mon_rect, &char_rect)) {
				crash_flag = true;
			}

			if (crash_flag) {
				crash_img_delay++;
				if (crash_img_delay > 4) {
					crash_img_delay = 0;
					crash_img_num++;
					if (crash_img_num >= 2) crash_img_num = 0;
					crash_delay++;

					if (crash_delay > 8) {
						crash_delay = 0;
						crash_flag = false;
					}
				}
			}

			//배경
			back_x[0] -= 2;
			back_x[1] -= 4;
			back_x[2] -= 8;

			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CHAR:
		switch (wParam) {
		case 'j':
			if (not char_jump_flag) {
				char_jump_flag = true;
				jump_v = initial_jump_v;
			}
			break;
		}
		break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		memdc = CreateCompatibleDC(hdc);

		//배경출력
		for (int i{}; i < 3; i++) {
			SelectObject(memdc, back[i]);
			GetObject(back[i], sizeof(BITMAP), &bit1);
			Bw = bit1.bmWidth;
			Bh = bit1.bmHeight;

			int scrollX = back_x[i] % Bw;
			if (scrollX < 0) scrollX += Bw;

			int first_width = Bw - scrollX;
			if (first_width > clientRect.right)
				first_width = clientRect.right;

			if (first_width > 0) {
				TransparentBlt(
					mdc,                    // 대상 DC
					0, 0,                   // 출력 위치
					first_width, Bh,        // 출력 크기
					memdc,                  // 원본 DC
					scrollX, 0,             // 원본 위치
					first_width, Bh,        // 원본 크기 (== 출력 크기)
					RGB(0, 0, 255)          // 투명색
				);
			}

			if (first_width < clientRect.right) {
				int second_width = clientRect.right - first_width;
				TransparentBlt(
					mdc,
					first_width, 0,
					second_width, Bh,
					memdc,
					0, 0,
					second_width, Bh,
					RGB(0, 0, 255)
				);
			}
		}

		//캐릭터출력
		if (crash_flag) {
			SelectObject(memdc, crash_image[crash_img_num]);
			TransparentBlt(mdc, char_x - 10, char_y + char_jump_y - 10, 81, 82, memdc, 0, 0, 81, 82, RGB(0, 0, 255));
		}

		SelectObject(memdc, character[char_order[char_img_num]]);
		if (not char_slide_flag) {
			TransparentBlt(mdc, char_x, char_y + char_jump_y, 80, 66, memdc, 0, 0, 80, 66, RGB(0, 0, 255));
		}
		else{
			TransparentBlt(mdc, char_x, char_y+33, 80, 66/2, memdc, 0, 0, 80, 66, RGB(0, 0, 255));
		}

		//몬스터출력
		SelectObject(memdc, monster[sel_monster][monster_img_num]);
		TransparentBlt(mdc, monster_x, monster_y, 90, 90, memdc, 0, 0, 90, 90, RGB(0, 0, 64));

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(memdc);
		DeleteObject(hbitmap);
		DeleteDC(mdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
