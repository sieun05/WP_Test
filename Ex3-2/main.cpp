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
		800,//CW_USEDEFAULT, //â�� ��
		800,//CW_USEDEFAULT, //â�� ����
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

	static int board[ROW][COLUMN]{0, }; //0��ĭ 1��ֹ� 2����
	static std::vector<Circle> shapes;
	static int main_circles_num{};

	static int fps{ 100 };

	switch (iMsg) {
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, fps, NULL); //Ÿ�̸� ����
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
			SetTimer(hWnd, 1, fps, NULL); //Ÿ�̸� ����
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
