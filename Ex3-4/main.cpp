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
	HDC hdc, mdc;
	HBITMAP hBitmap;

	PAINTSTRUCT ps;
	TCHAR prt_txt[100];

	static RECT clientRect{};
	
	static Cell board[ROW][COLUMN]{ 0,  };
	static std::vector<Block> blocks;

	static bool block_down_flag{};
	static bool game_end_flag{1};

	static int fps{ 1000 };

	static int n{};

	switch (iMsg) {
	case WM_CREATE:
	{
		
		break;
	}
	case WM_CHAR:
		switch (wParam) {
		case 's':
			game_end_flag = 0;	
			SetTimer(hWnd, 1, fps, NULL);
			memset(board, 0, sizeof(board));
			break;
		case 'q':
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;
		case VK_RETURN:
			if (blocks.size() > 0) {
				int temp{ blocks[blocks.size() - 1].width };
				blocks[blocks.size() - 1].width = blocks[blocks.size() - 1].height;
				blocks[blocks.size() - 1].height = temp;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN:
		if (not game_end_flag) {

			switch (wParam) {
			case VK_LEFT:
				Block_Move(board, blocks, -1, 0, block_down_flag, game_end_flag);
				break;
			case VK_RIGHT:
				Block_Move(board, blocks, 1, 0, block_down_flag, game_end_flag);
				break;
			case VK_UP:
				blocks[blocks.size() - 1].y = 0;
				break;
			case VK_DOWN:
				Block_Move(board, blocks, 0, 1, block_down_flag, game_end_flag);
				break;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		
		break;
	case WM_RBUTTONDOWN:
	{
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_TIMER:
	{
		if (game_end_flag) {
			memset(board, 0, sizeof(board));
			KillTimer(hWnd, 1);
			MessageBox(hWnd, TEXT("���� ����"), TEXT("Ex3-4"), MB_OK);
		}

		if (block_down_flag) {
			Block_Move(board, blocks, 0, 1, block_down_flag, game_end_flag);
			n += 10;
		}
		else {
			Block_Create(board, blocks, block_down_flag, game_end_flag); // �߰�
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

		RECT full_rect{ clientRect.left, clientRect.top, clientRect.left + clientRect.bottom - clientRect.bottom/5, clientRect.bottom };
		OffsetRect(&full_rect, clientRect.right / 2. - full_rect.right / 2., 0);
		InflateRect(&full_rect, -5, -5);

		GRID_PAINT(mdc, full_rect, board, blocks);

		wsprintf(prt_txt, TEXT("%d"), blocks.size());
		TextOut(mdc, 10, 10, prt_txt, lstrlen(prt_txt));
		if(blocks.size()>0){
			wsprintf(prt_txt, TEXT("%d %d"), blocks[blocks.size() - 1].x, blocks[blocks.size() - 1].y);
			TextOut(mdc, 10, 25, prt_txt, lstrlen(prt_txt));
		}
		wsprintf(prt_txt, TEXT("%d"), n);
		TextOut(mdc, 10, 40, prt_txt, lstrlen(prt_txt));

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
