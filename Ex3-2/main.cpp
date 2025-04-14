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
		1000,//CW_USEDEFAULT, //â�� ��
		1000,//CW_USEDEFAULT, //â�� ����
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

	static RECT clientRect;

	static int board[ROW][COLUMN]{0, };
	static std::vector<Circle> shapes;

	switch (iMsg) {
	case WM_CREATE:
	{
		shapes.emplace_back(0, 0, 0, 0, 0);
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
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
		
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &clientRect);

		GRID_PAINT(hdc, clientRect, board);

		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
