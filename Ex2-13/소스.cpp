#include "head.h"
#include "GRID_PAINT.h"

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
	PAINTSTRUCT ps;
	HDC hdc;

	TCHAR prt_txt[100];

	static int board[ROW][COLUMN]{0, };
	static Player player{};

	static int screen{};

	wstring Answer_Word[5]{L"HELLO", L"PROGRAMMING", L"WORLD", L"APPLE", L"BANANA"};
	static int Word_sel{};

	static int start_p{};
	static int game_end{};

	switch (iMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));

		if (screen == 0) {

			std::fill(&board[0][0], &board[0][0] + ROW * COLUMN, 0);

			for (int i{ 'A' }; i < 'Z'; i++) {
				for (int j{}; j < 2; j++) {
					int r_x{ rand() % 20 }, r_y{ rand() % 16 + 3 };
					if (board[r_y][r_x] == 0) {
						board[r_y][r_x] = i;
					}
				}
			}

			Word_sel = rand() % 5;
			start_p = (20 - Answer_Word[Word_sel].length()) / 2;
			int p{ start_p };

			for (int i{}; i < Answer_Word[Word_sel].length(); i++) {
				int flag{ rand() % 2 };
				if (flag) {
					board[1][p++] = Answer_Word[Word_sel][i];
				}
				else {
					p++;
				}
			}

			Player initial;
			player = initial;
			break;
		}
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 's':
		case 'S':
		{
			if (not screen)
				screen = 1;
			break;
		}
		}
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
		{
			MovePlayer(-1, 0, board, player);
			break;
		}
		case VK_RIGHT:
		{
			MovePlayer(1, 0, board, player);
			break;
		}
		case VK_UP:
		{
			MovePlayer(0, -1, board, player);
			break;
		}
		case VK_DOWN:
		{
			MovePlayer(0, 1, board, player);
			break;
		}
		}

		if (screen==1 and Word_Check(Answer_Word[Word_sel], board[1], start_p)) {
			if (game_end == 1) {
				screen = 0;
				game_end = 0;


				std::fill(&board[0][0], &board[0][0] + ROW * COLUMN, 0);

				for (int i{ 'A' }; i < 'Z'; i++) {
					for (int j{}; j < 2; j++) {
						int r_x{ rand() % 20 }, r_y{ rand() % 16 + 3 };
						if (board[r_y][r_x] == 0) {
							board[r_y][r_x] = i;
						}
					}
				}

				Word_sel = rand() % 5;
				start_p = (20 - Answer_Word[Word_sel].length()) / 2;
				int p{ start_p };

				for (int i{}; i < Answer_Word[Word_sel].length(); i++) {
					int flag{ rand() % 2 };
					if (flag) {
						board[1][p++] = Answer_Word[Word_sel][i];
					}
					else {
						p++;
					}
				}

				Player initial;
				player = initial;

				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}	
			game_end = 1;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		RECT client_rect;
		GetClientRect(hWnd, &client_rect);

		RECT full_rect{ client_rect.left, client_rect.top, client_rect.left + client_rect.bottom, client_rect.bottom };
		OffsetRect(&full_rect, client_rect.right/2. - full_rect.right/2. , 0);
		InflateRect(&full_rect, -5, -5);

		if (screen==0) {
			wsprintf(prt_txt, TEXT("Press 'S' to start"));
			DrawText(hdc, prt_txt, -1, &client_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if(screen == 1) {
			GRID_PAINT(hdc, full_rect, board, player);

			if (game_end) {
				MessageBox(NULL, TEXT("���� clear"), TEXT("WinAPI"), MB_OK);
			}
		}

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}