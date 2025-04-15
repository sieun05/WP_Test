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

	int fps{ 1000 };
	static int dx{ 0 };
	static int dy{ 0 };

	static int flag{}; //0�϶� ���� 1�϶� ���� 2�϶� ���� �̵�

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
