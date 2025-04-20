#include "head.h"

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
		1600,//CW_USEDEFAULT, //창의 폭
		900,//CW_USEDEFAULT, //창의 높이
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
	HDC hdc, mdc;
	HBITMAP hBitmap;
	PAINTSTRUCT ps;
	TCHAR prt_txt[100];

	static RECT clientRect;
	static RECT full_rect;

	static std::vector<Node> board;

	static int fps{ 300 };

	static Player Player1{ 0, RGB(255, 68, 86, false) };
	static Player Player2{ 0, RGB(142, 227, 50, false) };

	static int order{};
	static int Yut{};

	switch (iMsg) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &clientRect);
		full_rect={ clientRect.left, clientRect.top, clientRect.left + clientRect.bottom, clientRect.bottom };
		//OffsetRect(&full_rect, clientRect.right / 2. - full_rect.right / 2., 0);
		InflateRect(&full_rect, -100, -100);
		int width = (full_rect.right - full_rect.left)/5;
		int height = (full_rect.bottom - full_rect.top)/5;
		int x_width = (full_rect.right - full_rect.left) / 6;
		int x_height = (full_rect.bottom - full_rect.top) / 6;

		for (int i{}; i < 5; i++) {
			board.push_back({ full_rect.left + width*i, full_rect.bottom, {} });
		}
		for (int i{}; i < 5; i++) {
			board.push_back({ full_rect.right, full_rect.bottom - height * i, {} });
		}
		for (int i{}; i < 5; i++) {
			board.push_back({ full_rect.right - width * i, full_rect.top, {} });
		}
		for (int i{0}; i < 5; i++) {
			board.push_back({ full_rect.left, full_rect.top + height * i, {} });
		}

		//중앙
		board.push_back({ full_rect.left + (3 * x_width), full_rect.top + (3 * x_height), {} });

		//대각선
		for (int i{ 1 }; i < 6; i++) {
			if (i == 3) continue;
			board.push_back({ full_rect.right - x_width * i, full_rect.top + x_height * i, {} });
		}
		for (int i{ 1 }; i < 6; i++) {
			if (i == 3) continue;
			board.push_back({ full_rect.left + x_width * i, full_rect.top + x_height * i, {} });
		}

		//외각 경로 연결
		for (int i{}; i < 19; i++) {
			board[i].next.push_back(i + 1);
		}
		board[19].next = {}; //도착점

		//모서리 분기
		board[5].next.push_back(28);
		board[10].next.push_back(21);

		board[20].next.push_back(26);
		board[20].next.push_back(23);

		//지름길 오위>왼아
		board[21].next.push_back(22);
		board[22].next.push_back(20);
		board[23].next.push_back(24);
		board[24].next = {};

		board[26].next.push_back(25);
		board[25].next.push_back(15);
		board[28].next.push_back(27);
		board[27].next.push_back(20);

		SetTimer(hWnd, 1, 1000, NULL);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		MovePlayer(hWnd, Player1, Yut, board);
		if (Yut != 0 and Yut != 4) {
			order = 1;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 'p':

			break;
		case '1':
			//플레이어1 윷 던지기
			if (order == 0) {
				Yut = ThrowYut();
			}
			break;
		case '2':
			//ai 윷 던지기
			if(order==1){
				Yut = ThrowYut();
				MovePlayer(hWnd, Player2, Yut, board);
				if (Yut != 0 and Yut != 4) {
					order = 0;
				}
			}
			break;
		case 's':
			//게임 리셋하고 다시 시작
			Player1.pos = 0;
			Player1.finished = false;
			Player2.pos = 0;
			Player2.finished = false;
			order = 0;
			break;
		case 'q':
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_TIMER:
	{
		if (Player1.finished or Player2.finished) {
			Player1.pos = 0;
			Player1.finished = false;
			Player2.pos = 0;
			Player2.finished = false;
			order = 0;
			MessageBox(hWnd, TEXT("게임 종료"), TEXT("Ex3-3"), MB_OK);
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_PAINT:
	{
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &ps);
		mdc = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		SelectObject(mdc, (HBITMAP)hBitmap);
		Rectangle(mdc, 0, 0, clientRect.right, clientRect.bottom);

		//윷놀이 보드판
		DrawBoard(mdc, board, full_rect, Player1, Player2);

		RECT yut_rect{1000, 100, 1060, 250};
		for (int i{}; i < 4; i++) {
			HBRUSH hbrush = CreateSolidBrush(RGB(80, 43, 32));
			if (Yut - i > 0) {
				FrameRect(mdc, &yut_rect, hbrush);
			}
			else {
				FillRect(mdc, &yut_rect, hbrush);
			}
			DeleteObject(hbrush);
			OffsetRect(&yut_rect, 80, 0);
		}

		wsprintf(prt_txt, TEXT("player %d의 차례입니다."), order+1);
		TextOut(mdc, 1000, 70, prt_txt, lstrlen(prt_txt));

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);
		
		DeleteDC(mdc);
		DeleteObject(hBitmap);
		break;
	}
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

