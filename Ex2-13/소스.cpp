#include "head.h"
#include "GRID_PAINT.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR frm_title = TEXT("c_form_view");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

	HWND hWnd;
	MSG Msg_frm;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 창 스타일
	WndClass.lpfnWndProc = WndProc; //메세지 처리 함수로 지정
	WndClass.cbClsExtra = 0; //클래스 여분메모리: 예약영역으로 0으로 해주면 된다.
	WndClass.cbWndExtra = 0; //윈도우창 여분메모: 예약영역으로 0으로 해주면 된다.
	WndClass.hInstance = hInstance; //윈도우창 클래스를 등록하는 프로그램 번호
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 모양 결정(기본값)
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 모양 결정(기본값)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //윈도우창의 배경색상 지정
	WndClass.lpszMenuName = NULL; //윈도우창에서 메뉴 사용 결정
	WndClass.lpszClassName = frm_title; //윈도우 클래스 이름을 정의

	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		frm_title, //윈도우창 클래스를 지정
		TEXT("test"), //제목표시줄 문자열
		WS_OVERLAPPEDWINDOW,//윈도우창의 형태지정
		CW_USEDEFAULT, //가로 위치
		CW_USEDEFAULT, //세로 위치
		1280,//CW_USEDEFAULT, //창의 폭
		720,//CW_USEDEFAULT, //창의 높이
		NULL, //부모 윈도우창의 핸들을 지정
		(HMENU)NULL, //윈도우창에서 사용할 메뉴의 핸들을 지정
		hInstance, //프로그램의 핸들을 지정
		NULL); //구조체의 번지를 지정

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Msg_frm, NULL, 0, 0)) {
		TranslateMessage(&Msg_frm);
		DispatchMessage(&Msg_frm);
	}

	return (int)Msg_frm.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	TCHAR prt_txt[100];

	static int board[ROW][COLUMN]{0, };
	static Player player{};

	static int screen{};

	wstring Answer_Word[5]{L"HELLO", L"PROGRAMMING", L"WORLD", L"APPLE", L"BANANA"};
	static int Word_sel{};

	static int start_p{};
	static int game_end{};

	switch (iMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));

		if (screen == 0) {

			std::fill(&board[0][0], &board[0][0] + ROW * COLUMN, 0);

			for (int i{ 'A' }; i < 'Z'; i++) {
				for (int j{}; j < 2; j++) {
					int r_x{ rand() % 20 }, r_y{ rand() % 16 + 3 };
					if (board[r_y][r_x] == 0) {
						board[r_y][r_x] = i;
					}
				}
			}

			Word_sel = rand() % 5;
			start_p = (20 - Answer_Word[Word_sel].length()) / 2;
			int p{ start_p };

			for (int i{}; i < Answer_Word[Word_sel].length(); i++) {
				int flag{ rand() % 2 };
				if (flag) {
					board[1][p++] = Answer_Word[Word_sel][i];
				}
				else {
					p++;
				}
			}

			Player initial;
			player = initial;
			break;
		}
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 's':
		case 'S':
		{
			if (not screen)
				screen = 1;
			break;
		}
		}
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
		{
			MovePlayer(-1, 0, board, player);
			break;
		}
		case VK_RIGHT:
		{
			MovePlayer(1, 0, board, player);
			break;
		}
		case VK_UP:
		{
			MovePlayer(0, -1, board, player);
			break;
		}
		case VK_DOWN:
		{
			MovePlayer(0, 1, board, player);
			break;
		}
		}

		if (screen==1 and Word_Check(Answer_Word[Word_sel], board[1], start_p)) {
			if (game_end == 1) {
				screen = 0;
				game_end = 0;


				std::fill(&board[0][0], &board[0][0] + ROW * COLUMN, 0);

				for (int i{ 'A' }; i < 'Z'; i++) {
					for (int j{}; j < 2; j++) {
						int r_x{ rand() % 20 }, r_y{ rand() % 16 + 3 };
						if (board[r_y][r_x] == 0) {
							board[r_y][r_x] = i;
						}
					}
				}

				Word_sel = rand() % 5;
				start_p = (20 - Answer_Word[Word_sel].length()) / 2;
				int p{ start_p };

				for (int i{}; i < Answer_Word[Word_sel].length(); i++) {
					int flag{ rand() % 2 };
					if (flag) {
						board[1][p++] = Answer_Word[Word_sel][i];
					}
					else {
						p++;
					}
				}

				Player initial;
				player = initial;

				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}	
			game_end = 1;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		RECT client_rect;
		GetClientRect(hWnd, &client_rect);

		RECT full_rect{ client_rect.left, client_rect.top, client_rect.left + client_rect.bottom, client_rect.bottom };
		OffsetRect(&full_rect, client_rect.right/2. - full_rect.right/2. , 0);
		InflateRect(&full_rect, -5, -5);

		if (screen==0) {
			wsprintf(prt_txt, TEXT("Press 'S' to start"));
			DrawText(hdc, prt_txt, -1, &client_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if(screen == 1) {
			GRID_PAINT(hdc, full_rect, board, player);

			if (game_end) {
				MessageBox(NULL, TEXT("게임 clear"), TEXT("WinAPI"), MB_OK);
			}
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}