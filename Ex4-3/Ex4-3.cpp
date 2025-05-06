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
		1280,
		720,
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
	static RECT clientRect;
	static RECT f_rect;

	static int rect_width;
	static int rect_height;

	static Cell board[ROW][COL]{};

	static bool Hint_flag{};
	static int Hint_delay{};
	static bool Score_flag{};
	static int Score{};
	static int Item{};

	static bool reset_flag{};
	static bool game_end_flag{};
	static bool game_over_flag{};
	static bool game_check{};

	switch (iMessage) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 200, NULL);
		GetClientRect(hWnd, &clientRect);
		f_rect = { clientRect.left, clientRect.top, clientRect.left + (clientRect.bottom - clientRect.top), clientRect.bottom };
		rect_width={ (int)(f_rect.right - f_rect.left) / COL };
		rect_height={ (int)(f_rect.bottom - f_rect.top) / ROW };

		//지뢰
		for (int i{}; i < 20; i++) {
			int r_row{ row(gen) };
			int r_col{ col(gen) };

			if (not board[r_row][r_col].type) {
				board[r_row][r_col].type = 1;
			}
			else {
				i--;
				continue;
			}
		}

		//아이템
		for (int i{}; i < 10; i++) {
			int r_row{ row(gen) };
			int r_col{ col(gen) };

			if (not board[r_row][r_col].type) {
				board[r_row][r_col].type = 2;
			}
			else {
				i--;
				continue;
			}
		}

		//파이
		for (int i{}; i < 5; i++) {
			for (int j{}; j < 4; j++) {
				int r_row{ row(gen) };
				int r_col{ col(gen) };

				if (not board[r_row][r_col].type) {
					board[r_row][r_col].type = 3 + i;
					board[r_row][r_col].piece = j;
				}
				else {
					j--;
					continue;
				}
			}
		}
		break;
	}
	case WM_COMMAND:
		switch (wParam) {
		case ID_GAME_GAMESTART:
			reset_flag = true;
			break;
		case ID_GAME_GAMEEND:
			game_end_flag = true;
			break;
		case ID_HINT_HINT:
			Hint_flag = true;
			Hint_delay = 5*3;
			break;
		case ID_SCORE_SCOREON:
			Score_flag = true;
			break;
		case ID_SCORE_SCOREOFF:
			Score_flag = false;
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			Score_Check(board, Score);
			if(not game_check)
				Game_End_Check(board, game_end_flag, game_check);

			if (Hint_delay < 0) {
				Hint_flag = false;
				for (int i{}; i < ROW; i++) {
					for (int j{}; j < COL; j++) {
						board[i][j].hint_flag = false;
					}
				}
			}
			if (Hint_flag) {
				Hint_delay--;

				for (int i{}; i < ROW; i++) {
					for (int j{}; j < COL; j++) {
						board[i][j].hint_flag = true;
					}
				}
			}

			if (game_over_flag) {
				game_over_flag = false;
				int msg = MessageBox(hWnd, TEXT("Game Over\n* Restart? *"), TEXT("Game Over"), MB_YESNO);
				if (msg == IDYES) {
					reset_flag = true;
				}
				else {
					KillTimer(hWnd, 1);
					PostQuitMessage(0);
					return 0;
				}
			}

			if (game_end_flag) {
				game_end_flag = false;
				int msg = MessageBox(hWnd, TEXT("Game Clear\n* Restart? *"), TEXT("Game Clear"), MB_YESNO);
				if (msg == IDYES) {
					reset_flag = true;
				}
				else {
					KillTimer(hWnd, 1);
					PostQuitMessage(0);
					return 0;
				}
			}

			if (reset_flag) {
				reset_flag = false;

				memset(board, 0, sizeof(board));

				//지뢰
				for (int i{}; i < 20; i++) {
					int r_row{ row(gen) };
					int r_col{ col(gen) };

					if (not board[r_row][r_col].type) {
						board[r_row][r_col].type = 1;
					}
					else {
						i--;
						continue;
					}
				}

				//아이템
				for (int i{}; i < 10; i++) {
					int r_row{ row(gen) };
					int r_col{ col(gen) };

					if (not board[r_row][r_col].type) {
						board[r_row][r_col].type = 2;
					}
					else {
						i--;
						continue;
					}
				}

				//파이
				for (int i{}; i < 5; i++) {
					for (int j{}; j < 4; j++) {
						int r_row{ row(gen) };
						int r_col{ col(gen) };

						if (not board[r_row][r_col].type) {
							board[r_row][r_col].type = 3 + i;
							board[r_row][r_col].piece = j;
						}
						else {
							j--;
							continue;
						}
					}
				}

				Hint_flag={};
				Hint_delay={};
				Score_flag={};
				Score={};
				Item={};

				reset_flag={};
				game_end_flag={};
				game_over_flag={};
				game_check = {};
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
	{
		if(not Hint_flag){
			int mx{ LOWORD(lParam) };
			int my{ HIWORD(lParam) };

			int x{ (mx - clientRect.left) / rect_width };
			int y{ (my - clientRect.top) / rect_height };

			if (x < COL and y < ROW and not board[x][y].open_flag) {
				board[x][y].open_flag = true;
				if (board[x][y].type == 1) {
					game_over_flag = true;
				}
				else if(board[x][y].type >= 3 and board[x][y].type <= 7){
					if (Item >= 3 and Item <= 7)
						Item = board[x][y].type;
				}
				else if (board[x][y].type ==2) {
					if(Item>=3 and Item <=7){
						//같은 조각 열기
						for (int i{}; i < ROW; i++) {
							for (int j{}; j < COL; j++) {
								if (board[i][j].type == Item) {
									board[i][j].open_flag = true;
								}
							}
						}
					}
				}
			}
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
		f_rect = { clientRect.left, clientRect.top, clientRect.left + (clientRect.bottom - clientRect.top), clientRect.bottom };

		GRID_PAINT(mdc, f_rect, board);
		
		if (Score_flag) {
			TCHAR txt[100]{};
			wsprintf(txt, TEXT("Score: %d"), Score);
			TextOut(mdc, clientRect.right - 100, clientRect.top + 10, txt, lstrlen(txt));
		}

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hbitmap);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
