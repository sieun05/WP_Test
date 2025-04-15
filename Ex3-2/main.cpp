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
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR prt_txt[100];

	static RECT clientRect;

	static int board[ROW][COLUMN]{0, }; //0빈칸 1장애물 2먹이
	static std::vector<Circle> shapes;
	static int main_circles_num{};

	static int fps{ 100 };

	switch (iMsg) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, fps, NULL); //타이머 설정
		shapes.emplace_back(2, 0, 1, 0, ++main_circles_num, 0);
		shapes.emplace_back(1, 0, 1, 0, ++main_circles_num, 0);
		shapes.emplace_back(0, 0, 1, 0, ++main_circles_num, 3);

		shapes.emplace_back(7, 7, 1, 0, 0, 5);
		shapes.emplace_back(7, 9, 2, 0, 0, 5);
		shapes.emplace_back(10, 15, 3, 0, 0, 6);
		shapes.emplace_back(11, 15, 0, 0, 0, 6);

		board[0][0] = 10;
		board[0][1] = 10;
		board[0][2] = 10;
		board[7][7] = 10;
		board[7][9] = 10;
		board[10][15] = 10;
		board[11][15] = 10;

		for (int i{}; i < 30; i++) {
			int obstacleX{ dis(gen) };
			int obstacleY{ dis(gen) };
			if (not board[obstacleY][obstacleX]) {
				board[obstacleY][obstacleX] = 1;
			}
		}
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 'a':
			KillTimer(hWnd, 1);
			break;
		case 's':
			SetTimer(hWnd, 1, fps, NULL); //타이머 설정
			break;
		case 'q':
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
		
		break;
	case WM_TIMER:
	{
		Circle_Move(shapes, board);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &clientRect);

		GRID_PAINT(hdc, clientRect, board, shapes);

		break;
	}
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
