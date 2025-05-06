#include "head.h"


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
	static RECT R1_f_rect;
	static RECT R1_player_field[2];
	static RECT R2_f_rect;
	static RECT R2_player_field[2];

	static int centerX, centerY;

	static int round{};
	//static int R1_board[R1_ROW][R1_COL]{};
	static int R2_board[R2_ROW][R2_COL]{
		{0, 0, 1, 1, 1, 1, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 1, 1, 1, 1, 0, 0}
	};

	static int obstacle_move{};
	static float obstacle_speed{ 3 };

	static Obstacle R1_obstacle[12];

	static vector<Obstacle2> R2_obstacle;
	static std::vector<Obstacle> R2_obstacle_pos;
	static POINT R2_center; // 회전 중심
	static bool R2_clockwise = true;

	static int item_cnt{};
	static vector<POINT> item;
	static POINT player;

	static bool key4_flag{};
	static bool key5_flag{};
	static bool keyS_flag{};

	static bool reset_flag{};
	static bool game_end_flag{};
	static bool not_movekey_flag{};

	switch (iMessage) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 50, NULL);

		GetClientRect(hWnd, &clientRect);

		centerX = { (clientRect.left + clientRect.right) / 2 };
		centerY = { (clientRect.top + clientRect.bottom) / 2 };


		//R1
		{
			int R1_height{ 30 * R1_ROW };
			int R1_width{ 30 * R1_COL };

			R1_f_rect = {
					centerX - R1_width / 2, centerY - R1_height / 2,
					centerX + R1_width / 2, centerY + R1_height / 2 };
			R1_player_field[0] = { //시작지점
					R1_f_rect.left - 100, centerY - 30,
					R1_f_rect.left, centerY + 30 };
			R1_player_field[1] = { //끝지점
				R1_f_rect.right, centerY - 30,
				R1_f_rect.right + 100, centerY + 30 };

			for (int i{}; i < 12; i++) {
				R1_obstacle[i].color = 0;
				R1_obstacle[i].x = (R1_f_rect.left + 15) + 30 * i;
				if (i % 2 == 0)
					R1_obstacle[i].y = R1_f_rect.top + 6;
				else
					R1_obstacle[i].y = R1_f_rect.top + 174;
			}
			
		}
		
		//R2
		{
			int R1_height{ 30 * R2_ROW };
			int R1_width{ 30 * R2_COL };

			R2_f_rect = {
				centerX - R1_width / 2, centerY - R1_height / 2,
				centerX + R1_width / 2, centerY + R1_height / 2 };
			R2_player_field[0] = { //시작지점
				R2_f_rect.left - 100, centerY - 30,
				R2_f_rect.left, centerY + 30 };
			R2_player_field[1] = { //끝지점
				centerX-30, R2_f_rect.top-100,
				centerX+30, R2_f_rect.top };

			R2_center = {centerX, centerY};
			R2_obstacle.clear();
			R2_obstacle_pos.clear();

			const int spacing{ 25 }; //장애물 간격
			const int count{ 5 };

			for (int i = 1; i <= count; i++) {
				R2_obstacle.push_back({ -PI / 2, spacing * i, 0 }); // 위
				R2_obstacle.push_back({ PI / 2, spacing * i, 0 }); // 아래
				R2_obstacle.push_back({ PI,      spacing * i, 0 }); // 왼쪽
				R2_obstacle.push_back({ 0.0,          spacing * i, 0 }); // 오른쪽
			}

			obstacle_speed = 0.05;
		}
		break;
	}
	case WM_TIMER:
	{

		switch (wParam) {
		case 1:
		{
			int dx{}, dy{};

			if (GetAsyncKeyState(VK_LEFT) & 0x8000) dx -= 1;
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) dx += 1;
			if (GetAsyncKeyState(VK_UP) & 0x8000) dy -= 1;
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) dy += 1;

			if ((dx != 0 || dy != 0) and not not_movekey_flag) {
				if (round == 1)
					Player_Move(R1_f_rect, R1_player_field, nullptr, round, player, dx, dy, game_end_flag, item);
				else if (round == 2)
					Player_Move(R2_f_rect, R2_player_field, R2_board, round, player, dx, dy, game_end_flag, item);
			}

			if (round == 1) {
				Obstacle_Move(R1_obstacle, obstacle_move, obstacle_speed, R1_f_rect.top);
				Player_Crash(round, player, item, R1_obstacle, R2_obstacle_pos, reset_flag, keyS_flag);
			}
			else if (round == 2) {
				Obstacle_Move(R2_obstacle, R2_obstacle_pos, R2_center, obstacle_move, obstacle_speed);
				Player_Crash(round, player, item, R1_obstacle, R2_obstacle_pos, reset_flag, keyS_flag);
			}

			if (reset_flag) {
				switch (round) {
				case 1:
					player.x = R1_player_field[0].left + 10;
					player.y = R1_player_field[0].top + 20;

					for (int i{}; i < 12; i++) {
						R1_obstacle[i].color = 0;
						R1_obstacle[i].x = (R1_f_rect.left + 15) + 30 * i;
						if (i % 2 == 0)
							R1_obstacle[i].y = R1_f_rect.top + 6;
						else
							R1_obstacle[i].y = R1_f_rect.top + 174;
					}

					item.clear();
					item_cnt = 0;

					key4_flag = false;
					key5_flag = false;
					keyS_flag = false;

					obstacle_move = {};
					obstacle_speed = { 3 };
					break;
				case 2:
					player.x = R2_player_field[0].left + 10;
					player.y = R2_player_field[0].top + 20;

					R2_center = { centerX, centerY };
					R2_obstacle.clear();
					R2_obstacle_pos.clear();

					const int spacing{ 25 }; //장애물 간격
					const int count{ 4 };

					for (int i = 1; i <= count; i++) {
						R2_obstacle.push_back({ -PI / 2, spacing * i, 0 }); // 위
						R2_obstacle.push_back({ PI / 2, spacing * i, 0 }); // 아래
						R2_obstacle.push_back({ PI,      spacing * i, 0 }); // 왼쪽
						R2_obstacle.push_back({ 0.0,          spacing * i, 0 }); // 오른쪽
					}

					item.clear();
					item_cnt = 0;

					key4_flag = false;
					key5_flag = false;
					keyS_flag = false;

					obstacle_move = {};
					obstacle_speed = { 0.05 };
					break;
				}
				reset_flag = false;
				not_movekey_flag = false;
			}

			if (game_end_flag) {	
				not_movekey_flag = true;
				game_end_flag = false;
				int msgbox = MessageBox(hWnd, TEXT("Game Clear\n* restart? *"), TEXT("Game Clear"), MB_YESNO);
				if (msgbox == IDYES) {
					reset_flag = true;
					round = 0;
				}
				else {
					KillTimer(hWnd, 1);
					PostQuitMessage(0);
					return 0;
				}
			}
			break;
		}
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_CHAR:
		switch (wParam) {
		case '1':
			if (round == 0) {
				round = 1;
				player.x = R1_player_field[0].left + 10;
				player.y = R1_player_field[0].top + 20;
				reset_flag = true;
			}
			break;
		case '2':
			if (round == 0) {
				round = 2;
				player.x = R2_player_field[0].left + 10;
				player.y = R2_player_field[0].top + 20;
				reset_flag = true;
			}
			break;
		case '3':
			obstacle_move = obstacle_move ? 0 : 1;
			break;
		case '4':
		{
			int obstacle_cnt = (round == 1) ? 12 : R2_obstacle.size();

			if (round == 1) {
				if (not key4_flag) {
					for (int i = 0; i < obstacle_cnt; i++) {
						int ran = dis(gen);
						if (ran) {
							int rcolor = rcol(gen);
							R1_obstacle[i].color = rcolor;
						}
					}
					key4_flag = true;
				}
				else {
					for (int i = 0; i < obstacle_cnt; i++) {
						R1_obstacle[i].color = 0;
					}
					key4_flag = false;
				}
			}
			
			if (round == 2) {
				if (not key4_flag) {
					for (int i = 0; i < obstacle_cnt; i++) {
						int ran = dis(gen);
						if (ran) {
							int rcolor = rcol(gen);
							R2_obstacle[i].color = rcolor;
						}
					}
					key4_flag = true;
				}
				else {
					for (int i = 0; i < obstacle_cnt; i++) {
						R2_obstacle[i].color = 0;
					}
					key4_flag = false;
				}
			}
			break;
		}
		case '5':
			if (not key5_flag) {
				KillTimer(hWnd, 1);
				key5_flag = true;
			}
			else {
				SetTimer(hWnd, 1, 50, NULL);
				key5_flag = false;
			}
			break;
		case 's':
			keyS_flag = keyS_flag ? false : true;
			break;
		case 'r':
			reset_flag = true;
			break;
		case '+':
			if(round==1)
				obstacle_speed++;
			if (round == 2)
				obstacle_speed += 0.01;
			break;
		case '-':
			if (obstacle_speed > 0 and round == 1)
				obstacle_speed--;
			if (obstacle_speed > 0.01 and round == 2)
				obstacle_speed -= 0.01;
			
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
		if (item_cnt < 5) {
			POINT mp;
			mp.x = LOWORD(lParam);
			mp.y = HIWORD(lParam);

			if (round == 1) {
				RECT f_rect_border = { R1_f_rect };
				InflateRect(&f_rect_border, -5, -5);

				if (PtInRect(&f_rect_border, mp)) {
					item.emplace_back(mp);
					item_cnt++;
				}
			}
			else if (round == 2) {
				double r{ sqrt(11700) - 5 };
				if (InCircle(centerX, centerY, mp.x, mp.y, r)) {
					item.emplace_back(mp);
					item_cnt++;
				}
			}
		}
		break;
	}
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, hbitmap);
		HBRUSH hbrush = CreateSolidBrush(RGB(179, 186, 255));
		FillRect(mdc, &clientRect, hbrush);
		DeleteObject(hbrush);

		switch (round) {
		case 0:

			break;
		case 1:
		{
			GRID_PAINT(mdc, R1_f_rect, R1_player_field, nullptr, round, R1_obstacle, R2_obstacle_pos, player, item);
			break;
		}
		case 2:
			GRID_PAINT(mdc, R2_f_rect, R2_player_field, R2_board, round, R1_obstacle, R2_obstacle_pos, player, item);
			break;
		}

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hbitmap);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
