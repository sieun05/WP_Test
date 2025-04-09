#include "head.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR frm_title = TEXT("c_form_view");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

	HWND hWnd;
	MSG Msg_frm;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW; //������ â ��Ÿ��
	WndClass.lpfnWndProc = WndProc; //�޼��� ó�� �Լ��� ����
	WndClass.cbClsExtra = 0; //Ŭ���� ���и޸�: ���࿵������ 0���� ���ָ� �ȴ�.
	WndClass.cbWndExtra = 0; //������â ���и޸�: ���࿵������ 0���� ���ָ� �ȴ�.
	WndClass.hInstance = hInstance; //������â Ŭ������ ����ϴ� ���α׷� ��ȣ
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ ��� ����(�⺻��)
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ��� ����(�⺻��)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������â�� ������ ����
	WndClass.lpszMenuName = NULL; //������â���� �޴� ��� ����
	WndClass.lpszClassName = frm_title; //������ Ŭ���� �̸��� ����

	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		frm_title, //������â Ŭ������ ����
		TEXT("test"), //����ǥ���� ���ڿ�
		WS_OVERLAPPEDWINDOW,//������â�� ��������
		CW_USEDEFAULT, //���� ��ġ
		CW_USEDEFAULT, //���� ��ġ
		1280,//CW_USEDEFAULT, //â�� ��
		720,//CW_USEDEFAULT, //â�� ����
		NULL, //�θ� ������â�� �ڵ��� ����
		(HMENU)NULL, //������â���� ����� �޴��� �ڵ��� ����
		hInstance, //���α׷��� �ڵ��� ����
		NULL); //����ü�� ������ ����

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
