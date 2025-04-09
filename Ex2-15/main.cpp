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

	static std::vector<Shape> shapes;
	static int select_index{-1};
	TCHAR shape[5][100]{ L"Line", L"Triangle", L"Rectangle", L"Pentagon", L"Hexagon"};

	static int all_shapes_print{};
	static std::wstring memo;

	switch (iMsg) {
	case WM_CREATE:

		shapes.emplace_back(2, 100, 100, 300, 300, 3, 2, 2);
		select_index = shapes.size()-1;
		shapes.emplace_back(4, 200, 200, 300, 300, 5, 3, 5);

		break;
	case WM_CHAR:
		switch (wParam) {
		case '+':
			if (select_index > -1) {
				if (shapes[select_index].width > 10)
					shapes[select_index].size++;
				else shapes[select_index].width++;
			}
			break;
		case '-':
			if (select_index > -1) {
				if (shapes[select_index].width < 2)
					shapes[select_index].size--;
				else if (shapes[select_index].width > - 10)
					shapes[select_index].width--;
			}
			break;
		case '1':
			if (select_index > -1) {
				shapes[select_index].pen_color++;
				if (shapes[select_index].pen_color > 4)
					shapes[select_index].pen_color %= 5;
			}
			break;
		case '2':
			if (select_index > -1) {
				shapes[select_index].brush_color++;
				if (shapes[select_index].brush_color > 4)
					shapes[select_index].brush_color %= 5;
			}
			break;
		case 'p':
			if (select_index > 0)
				select_index--;
			break;
		case 'n':
			if (select_index < shapes.size() - 1)
				select_index++;
			break;
		case 'a':
			all_shapes_print = (all_shapes_print == 0) ? 1 : 0;
			break;
		case 'd':

			break;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		defualt:
			memo += wParam;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			Shape_Move(-1, 0, shapes, select_index);
			break;
		case VK_RIGHT:
			Shape_Move(1, 0, shapes, select_index);
			break;
		case VK_UP:
			Shape_Move(0, -1, shapes, select_index);
			break;
		case VK_DOWN:
			Shape_Move(0, 1, shapes, select_index);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		Shape_PAINT(hdc, shapes, select_index, all_shapes_print);

		if(not all_shapes_print){
			Shape& s = shapes[select_index];

			wsprintf(prt_txt, TEXT("%s"),memo);
			TextOut(hdc, clientRect.right - 200, 70, prt_txt, lstrlen(prt_txt));
			wsprintf(prt_txt, shape[s.shape_sel - 2]);
			TextOut(hdc, clientRect.right - 200, 100, prt_txt, lstrlen(prt_txt));
			wsprintf(prt_txt, TEXT("Point: (%d, %d), (%d, %d)"),s.x1, s.y1, s.x2, s.y2);
			TextOut(hdc, clientRect.right - 200, 120, prt_txt, lstrlen(prt_txt));
			wsprintf(prt_txt, TEXT("Width: %d"), s.width);
			TextOut(hdc, clientRect.right - 200, 140, prt_txt, lstrlen(prt_txt));
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
