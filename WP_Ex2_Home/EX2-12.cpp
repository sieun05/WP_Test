#include "head.h"
#include "GRID_PAINT.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR frm_title = TEXT("c_form_view");
HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    HWND hWnd;
    MSG Msg_frm;
    WNDCLASS WndClass;

    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //윈도우창의 배경색상 지정 //CreateSolidBrush(RGB(255,0,0));
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = frm_title;

    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        frm_title, //윈도우창 클래스 이름
        TEXT("Ex2-7"), //윈도우의 타이틀 이름 (제목표시줄 문자열)
        (WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_BORDER),//윈도우창의 형태지정 //(WS_CAPTION|WS_SYSMENU|WS_MAXIMIZEBOX|WS_THICKFRAME)
        CW_USEDEFAULT, //가로 위치
        CW_USEDEFAULT, //세로 위치
        1280,//CW_USEDEFAULT, //창의 폭
        720,//CW_USEDEFAULT, //창의 높이
        NULL, //부모 윈도우창의 핸들을 지정
        (HMENU)NULL, //윈도우창에서 사용할 메뉴의 핸들을 지정
        hInstance, //프로그램의 핸들을 지정
        NULL //구조체의 번지를 지정
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Msg_frm, NULL, 0, 0)) {
        TranslateMessage(&Msg_frm);
        DispatchMessage(&Msg_frm);
    }

    return (int)Msg_frm.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    srand((unsigned)time(NULL));

    PAINTSTRUCT ps;
    HDC hdc;

    HBRUSH hBrush;
    HBRUSH old_brush;

    HPEN hPen;
    HPEN old_pen;

	static int board[ROW][COLUMN]{ 0, }; 
    //player1: 1, player2: 2, 장애물: 3, 색상변경칸: 4, 5, 6, 모양변경칸 7, 크기변경칸: 8, 9

    static Player player[2]{ {0, 0}, { 39, 39 } };
	static int order{}; //현재 턴을 가진 플레이어

    switch (iMsg) {
    case WM_CREATE:
    {
    }
        break;
    case WM_CHAR:
    {
        switch (wParam) {
        case 't':

            break;
        case 'q':
        case 'Q':
            PostQuitMessage(0);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {

        case VK_LEFT:
			if (player[order].getPoint().x > 0) //왼쪽으로 이동
				player[order].setPoint(player[order].getPoint().x - 1, player[order].getPoint().y); //플레이어의 좌표를 변경
			else
				player[order].setPoint(0, player[order].getPoint().y); //플레이어의 좌표를 변경
            break;
        case VK_RIGHT:
			if (player[order].getPoint().x < COLUMN - 1) //오른쪽으로 이동
				player[order].setPoint(player[order].getPoint().x + 1, player[order].getPoint().y); //플레이어의 좌표를 변경
			else
				player[order].setPoint(COLUMN - 1, player[order].getPoint().y); //플레이어의 좌표를 변경
            break;
        case VK_UP:
			if (player[order].getPoint().y > 0) //위쪽으로 이동
				player[order].setPoint(player[order].getPoint().x, player[order].getPoint().y - 1); //플레이어의 좌표를 변경
			else
				player[order].setPoint(player[order].getPoint().x, 0); //플레이어의 좌표를 변경
            break;
        case VK_DOWN:
			if (player[order].getPoint().y < ROW - 1) //아래쪽으로 이동
				player[order].setPoint(player[order].getPoint().x, player[order].getPoint().y + 1); //플레이어의 좌표를 변경
			else
				player[order].setPoint(player[order].getPoint().x, ROW - 1); //플레이어의 좌표를 변경
            break;
        }
        board[player[order].getPoint().x][player[order].getPoint().y] = 1; //플레이어1
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        RECT full_rect{client_rect.left, client_rect.top, client_rect.left+ client_rect.bottom-10, client_rect.bottom-10};
        OffsetRect(&full_rect, 5, 5);

        GRID_PAINT(hdc, full_rect, board, player); //그리드 그리기


        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

