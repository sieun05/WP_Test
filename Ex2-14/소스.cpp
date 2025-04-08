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
	HDC hdc;
	PAINTSTRUCT ps;

	static int board[ROW][COLUMN]{0, };

	static vector<Shape> shapes;
	static int select_index{-1};

	TCHAR prt_txt[100]{};

	static int flag_c{};

	switch (iMsg) {
	//case WM_CREATE:
	//{


	//	break;
	//}
	case WM_CHAR:
	{
		switch (wParam) {
		case 'e':
			Shape_Paint(1, shapes, board);
			break;
		case 't':
			Shape_Paint(2, shapes, board);
			break;
		case 'r':
			Shape_Paint(3, shapes, board);
			break;
		case '+':
			if (select_index > -1) {
				shapes[select_index].size++;
			}
			break;
		case '-':
			if (select_index > -1) {
				if(shapes[select_index].size>-5){
					shapes[select_index].size--;
				}
			}
			break;
		case 'c':
		{
			if(not flag_c){
				for (int i{}; i < shapes.size(); i++) {
					if (shapes[i].shape_sel == shapes[select_index].shape_sel) {
						shapes[i].flag_c = 1;
						
					}
				}
				flag_c = 1;
			}
			else {
				for (int i{}; i < shapes.size(); i++) {
					if (shapes[i].flag_c)
						shapes[i].flag_c = 0;
					
				}
				flag_c = 0;
			}
			break;
		}
		case 'd':
		{
			if (select_index > -1) {
				Shape_Delete(select_index, shapes, board);
				select_index = -1;
			}
			break;
		}
		case 'p':
		{
			for (int i{}; i < shapes.size(); i++) {
				Shape_Delete(i, shapes, board);
			}
			break;
		}
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		}

		if (wParam >= '1' and wParam <= '9') {
			select_index = wParam - '1';
		}
		else if (wParam == '0') {
			select_index = 9;
		}
			
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
			Shape_Move(-1, 0, select_index, shapes, board);
			break;
		case VK_RIGHT:
			Shape_Move(1, 0, select_index, shapes, board);
			break;
		case VK_UP:
			Shape_Move(0, -1, select_index, shapes, board);
			break;
		case VK_DOWN:
			Shape_Move(0, 1, select_index, shapes, board);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		RECT client_rect;
		GetClientRect(hWnd, &client_rect);

		RECT full_rect{client_rect.left, client_rect.top, client_rect.bottom, client_rect.bottom};
		OffsetRect(&full_rect, client_rect.right/2. - full_rect.right / 2., 0);
		InflateRect(&full_rect, -5, -5);

		GRID_PAINT(hdc, full_rect, board, shapes, select_index);

		wsprintf(prt_txt, TEXT("%d"), (int)shapes.size());
		TextOut(hdc, client_rect.right - 50, 100, prt_txt, lstrlen(prt_txt));

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
