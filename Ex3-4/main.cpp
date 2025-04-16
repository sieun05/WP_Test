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
		800,//CW_USEDEFAULT, //창의 폭
		800,//CW_USEDEFAULT, //창의 높이
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

	static RECT clientRect{};
	
	static Cell board[ROW][COLUMN]{ 0,  };
	static std::vector<Block_1> b1;
	static std::vector<Block_2> b2;
	static std::vector<Block_3> b3;

	static int block_down_flag{};
	static int game_end_flag{};
	static int current_block{};
	static int current_block_index{};

	static int fps{ 300 };

	switch (iMsg) {
	case WM_CREATE:
	{
		
		break;
	}
	case WM_CHAR:
		switch (wParam) {
		case 's':
			SetTimer(hWnd, 1, fps, NULL);
			break;
		case 'q':
			//KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;
		case VK_RETURN:

			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:

			break;
		case VK_RIGHT:

			break;
		case VK_UP:

			break;
		case VK_DOWN:

			break;
		}
		break;
	case WM_LBUTTONDOWN:
		
		break;
	case WM_RBUTTONDOWN:
	{
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_TIMER:
	{
		if (not block_down_flag) {
			Block_Create(board, b1, b2, b3, current_block, current_block_index, block_down_flag, game_end_flag);
			//Crash_Check(board, b1, b2, b3);
		}
		else {
			Block_Move(board, b1, b2, b3, current_block, current_block_index);
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

		RECT full_rect{ clientRect.left, clientRect.top, clientRect.left + clientRect.bottom - clientRect.bottom/5, clientRect.bottom };
		OffsetRect(&full_rect, clientRect.right / 2. - full_rect.right / 2., 0);
		InflateRect(&full_rect, -5, -5);

		GRID_PAINT(mdc, full_rect, board);

		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

		DeleteDC(mdc);
		DeleteObject(hBitmap);
		break;
	}
	case WM_DESTROY:
		//KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
