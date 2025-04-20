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
		1600,//CW_USEDEFAULT, //â�� ��
		900,//CW_USEDEFAULT, //â�� ����
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

		//�߾�
		board.push_back({ full_rect.left + (3 * x_width), full_rect.top + (3 * x_height), {} });

		//�밢��
		for (int i{ 1 }; i < 6; i++) {
			if (i == 3) continue;
			board.push_back({ full_rect.right - x_width * i, full_rect.top + x_height * i, {} });
		}
		for (int i{ 1 }; i < 6; i++) {
			if (i == 3) continue;
			board.push_back({ full_rect.left + x_width * i, full_rect.top + x_height * i, {} });
		}

		//�ܰ� ��� ����
		for (int i{}; i < 19; i++) {
			board[i].next.push_back(i + 1);
		}
		board[19].next = {}; //������

		//�𼭸� �б�
		board[5].next.push_back(28);
		board[10].next.push_back(21);

		board[20].next.push_back(26);
		board[20].next.push_back(23);

		//������ ����>�޾�
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
			//�÷��̾�1 �� ������
			if (order == 0) {
				Yut = ThrowYut();
			}
			break;
		case '2':
			//ai �� ������
			if(order==1){
				Yut = ThrowYut();
				MovePlayer(hWnd, Player2, Yut, board);
				if (Yut != 0 and Yut != 4) {
					order = 0;
				}
			}
			break;
		case 's':
			//���� �����ϰ� �ٽ� ����
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
			MessageBox(hWnd, TEXT("���� ����"), TEXT("Ex3-3"), MB_OK);
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

		//������ ������
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

		wsprintf(prt_txt, TEXT("player %d�� �����Դϴ�."), order+1);
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

