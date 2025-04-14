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
	TCHAR prt_txt[100];

	int fps{ 1000 };
	static int dx{ 0 };
	static int dy{ 0 };

	static int flag{}; //0일때 정지 1일때 가로 2일때 세로 이동

	static RECT clientRect;
	static RECT rect;

	static int mx, my;

	switch (iMsg) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 100, NULL);
		GetClientRect(hWnd, &clientRect);
		rect.left = clientRect.left;
		rect.top = clientRect.top;
		rect.right = rect.left + 50;
		rect.bottom =rect.top+50;

		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 'h':
			if (flag != 1) {
				flag = 1;
				dy != 0 ? dx = dy : dx = 3;
				dy = 0;
			}
			break;
		case 'v':
			if (flag != 2) {
				flag = 2;
				dx != 0 ? dy = dx : dy = 3;
				dx = 0;
			}
			break;
		case 'p':
			dx = 0;
			dy = 0;
			flag = 0;
			break;
		case '+':
			if (flag == 1) {
				dx > 0 ? dx++ : dx--;
			}
			else if (flag == 2) {
				dy > 0 ? dy++ : dy--;
			}
			break;
		case '-':
			if (flag == 1) {
				dx = (dx > 0 ? dx-- : dx++);
			}
			else if (flag == 2) {
				dy = (dy > 0 ? dy-- : dy++);
			}
			break;
		case 'q':
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		rect.left = mx - 25;
		rect.right = mx + 25;
		rect.top = my - 25;
		rect.bottom = my + 25;
		break;
	case WM_TIMER:
		OffsetRect(&rect, dx, dy);
		if (rect.right > clientRect.right) {
			OffsetRect(&rect, -(rect.right - clientRect.right), 50);
			dx *= -1;
		}
		if (rect.left < clientRect.left) {
			OffsetRect(&rect, clientRect.left - rect.left, 50);
			dx *= -1;
		}
		if (rect.top < clientRect.top) {
			OffsetRect(&rect, 50, clientRect.top - rect.top);
			dy *= -1;
		}
		if (rect.bottom > clientRect.bottom) {
			OffsetRect(&rect, 50, -(rect.bottom - clientRect.bottom));
			dy *= -1;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		GetClientRect(hWnd, &clientRect);

		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
