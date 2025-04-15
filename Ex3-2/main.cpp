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
		800,//CW_USEDEFAULT, //창의 폭
		800,//CW_USEDEFAULT, //창의 높이
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

	static RECT clientRect;

	static Cell board[ROW][COLUMN]{0, }; //0빈칸 1장애물 2먹이
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
				board[obstacleY][obstacleX].type = 2; //이 자리에 숫자를 하나 더 저장할 방법?
				board[obstacleY][obstacleX].color = color(gen);
			}
		}
		break;
	}
	case WM_CHAR:
	{
		switch (wParam) {
		case 's':
			SetTimer(hWnd, 1, fps, NULL); //타이머 설정
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
			//jump_up_flag, jump_down_flag, jump_x, jump_y << 평소 x,y=0 점프할때만 조정
			break;
		case 't':
			//order만 한 칸씩 이동하면 됨
			break;
		case 'a':
			//고민좀 해봐야함...... 그냥 꼬리 벡터를 정말 하나 더 만들어야하나 싶음
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
		//클릭햇을때 위치 구하고 주인공원이랑 위치 비교 (같으면 원 크기 크게, 다르면 이동방향 바꾸기)
		//이동방향 바꾸는 방식: board[y][x]=4 5 6 7??? 아니면 4만 하고 장애물처럼 처리한다음에 방향만 조정하기?? 키보드 입력방식이랑 같이 고민하기
		//꼬리원이라면 그 뒤 꼬리원 다 order=0 만들고 direction 랜덤값 주기
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
