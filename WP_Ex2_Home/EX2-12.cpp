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

    static Player game_end( 20, 19, -4, 200, 0, 200 );

    TCHAR prt_txt[100];

    static int end_check{};

    switch (iMsg) {
    case WM_CREATE:
    {
        board[0][0] = 11, board[39][39]=11;
        board[19][20] = 10; // 도착지점

        for (int i{}; i < 20; i++) {
            int r_x{ rand() % 39 }, r_y{ rand() % 39 };
            if (board[r_y][r_x] == 0) {
                board[r_y][r_x] = 3;
                continue;
            }
            i--;
        }

        for (int i{}; i < 15; i++) {
            int r_x{ rand() % 39 }, r_y{ rand() % 39 };
            int r_color{ rand() % 3 + 4};
            if (board[r_y][r_x] == 0) {
                board[r_y][r_x] = r_color;
                continue;
            }
            i--;
        }

        for (int i{}; i < 10; i++) {
            int r_x{ rand() % 39 }, r_y{ rand() % 39 };
            int r_shape{ 7 };
            if (board[r_y][r_x] == 0) {
                board[r_y][r_x] = r_shape;
                continue;
            }
            i--;
        }

        for (int i{}; i < 10; i++) {
            int r_x{ rand() % 39 }, r_y{ rand() % 39 };
            int r_size{ rand()%2 + 8};
            if (board[r_y][r_x] == 0) {
                board[r_y][r_x] = r_size;
                continue;
            }
            i--;
        }
    }
        break;
    case WM_CHAR:
    {
        bool moved = false;
        switch (wParam) {
        case 'a':
            if (order==0 and player[0].x > 0 and board[player[0].y][player[0].x-1]!=3) {
                player[0].x--;
                moved = true;
            }
            break;
        case 'd':
            if (order == 0 and player[0].x < COLUMN - 1 and board[player[0].y][player[0].x + 1] != 3) {
                player[0].x++;
                moved = true;
            }
            break;
        case 'w':
            if (order == 0 and player[0].y > 0 and board[player[0].y-1][player[0].x] != 3) {
                player[0].y--;
                moved = true;
            }
            break;
        case 's':
            if (order == 0 and player[0].y < ROW - 1 and board[player[0].y+1][player[0].x] != 3) {
                player[0].y++;
                moved = true;
            }
            break;
        case 'q':
        case 'Q':
            PostQuitMessage(0);
            break;
        }

        if (moved) {
            if (player[0] == game_end) {
                end_check = 1;
            }

            int cell = board[player[0].y][player[0].x];
            UpdatePlayerState(player[0], cell);
            order = 1;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYDOWN:
    {   
        bool moved = false;
        switch (wParam) {
        case VK_LEFT:
            if (order == 1 and player[1].x > 0 and board[player[1].y][player[1].x - 1] != 3) {
                player[1].x--;
                moved = true;
            }

            break;
        case VK_RIGHT:
            if (order == 1 and player[1].x < COLUMN - 1 and board[player[1].y][player[1].x + 1] != 3) {
                player[1].x++;
                moved = true;
            }
            break;
        case VK_UP:
            if (order == 1 and player[1].y > 0 and board[player[1].y - 1][player[1].x] != 3) {
                player[1].y--;
                moved = true;
            }
            break;
        case VK_DOWN:
            if (order == 1 and player[1].y < ROW - 1 and board[player[1].y + 1][player[1].x] != 3) {
                player[1].y++;
                moved = true;
            }
            break;
        }

        if (moved) {
            if (player[1] == game_end) {
                end_check = 1;
            }

            int cell = board[player[1].y][player[1].x];
            UpdatePlayerState(player[1], cell);
            order = 0;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        RECT full_rect{ client_rect.left, client_rect.top, client_rect.left + client_rect.bottom - 10, client_rect.bottom - 10 };
        OffsetRect(&full_rect, 5, 5);
        
        GRID_PAINT(hdc, full_rect, board, player, game_end); //그리드 그리기
        wsprintf(prt_txt, TEXT("order: %d"), order);
        TextOut(hdc, client_rect.right-100, 100, prt_txt, lstrlen(prt_txt));

        if (end_check) {
            MessageBox(NULL, TEXT("게임 clear"), TEXT("WinAPI"), MB_OK);
            end_check = 0;
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

