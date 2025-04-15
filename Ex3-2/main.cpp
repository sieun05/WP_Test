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

	static Cell board[ROW][COLUMN]{0, }; //0��ĭ 1��ֹ� 2����
	static std::vector<Circle> shapes;
	static int main_circles_num{};


	static int fps{ 300 };
	static int obstacle_num{};

	switch (iMsg) {
	case WM_CREATE:
	{
		//shapes.emplace_back(1, 0, 1, 0, ++main_circles_num, 0);
		shapes.emplace_back(0, 0, 1, 0, ++main_circles_num, 3);

		board[0][0].type = 10;

		/*for (int i{}; i < 20; i++) {
			int obstacleX{ dis(gen) };
			int obstacleY{ dis(gen) };
			if (not board[obstacleY][obstacleX].type) {
				board[obstacleY][obstacleX].type = 1;
			}
		}*/

		for (int i{}; i < 50; i++) {
			int obstacleX{ dis(gen) };
			int obstacleY{ dis(gen) };
			if (not board[obstacleY][obstacleX].type) {
				board[obstacleY][obstacleX].type = 2; //�� �ڸ��� ���ڸ� �ϳ� �� ������ ���?
				board[obstacleY][obstacleX].color = color(gen);
			}
		}
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 's':
			SetTimer(hWnd, 1, fps, NULL); //Ÿ�̸� ����
			break;
		case '+':
			KillTimer(hWnd, 1);
			fps -= 30;
			SetTimer(hWnd, 1, fps, NULL);
			break;
		case '-':
			KillTimer(hWnd, 1);
			fps += 30;
			SetTimer(hWnd, 1, fps, NULL);
			break;
		case 'j':
			//jump_up_flag, jump_down_flag, jump_x, jump_y << ��� x,y=0 �����Ҷ��� ����
			break;
		case 't':
			//order�� �� ĭ�� �̵��ϸ� ��
			break;
		case 'a':
			//����� �غ�����...... �׳� ���� ���͸� ���� �ϳ� �� �������ϳ� ����
			break;
		case 'q':
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		//Ŭ�������� ��ġ ���ϰ� ���ΰ����̶� ��ġ �� (������ �� ũ�� ũ��, �ٸ��� �̵����� �ٲٱ�)
		//�̵����� �ٲٴ� ���: board[y][x]=4 5 6 7??? �ƴϸ� 4�� �ϰ� ��ֹ�ó�� ó���Ѵ����� ���⸸ �����ϱ�?? Ű���� �Է¹���̶� ���� ����ϱ�
		//�������̶�� �� �� ������ �� order=0 ����� direction ������ �ֱ�
		break;
	case WM_RBUTTONDOWN:
	{
		if(obstacle_num<20){
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			float rect_width{ (float)(clientRect.right - clientRect.left) / COLUMN };
			float rect_height{ (float)(clientRect.bottom - clientRect.top) / ROW };

			int o_x = (mx - clientRect.left) / rect_width;
			int o_y = (my - clientRect.top) / rect_height;

			if(not board[o_y][o_x].type){
				board[o_y][o_x].type = 1;
				obstacle_num++;
			}
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_TIMER:
	{
		Circle_Move(shapes, board, main_circles_num);

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
