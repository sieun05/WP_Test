#include "head.h"


#define marginX 16
#define marginY 39



HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2025 Window Programming Exam";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int  WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR lpszCmdParam, _In_  int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);

    //--- 크기 변경 가능 (기존 (1024, 768))
    hWnd = CreateWindow(
        lpszClass,
        lpszWindowName,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        1600,
        900,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, mdc;
    PAINTSTRUCT ps;
    HBITMAP hBitmap;
    TCHAR txt[100];

    static RECT clientRect;

    static int traffic_light{}; //0빨강 1노랑 2초록
    static int light_delay{};
    int keep_time[4]{ 10, 2, 10, 2 };

    static RECT player{};
    static POINT player_p{};
    static int player_move{ 1 };
    static bool player_move_flag{ false };

    static Car car[8]{
        {-1, 0, 3}, //좌
        {-1, -140, 2},
        {1, 0, 2}, //우
        {1, -140, 0},
        {-2, 0, 2}, //위
        {-2, -140, 1},
        {2, 0, 3}, //아래
        {2, -140, 1}
    };
    static int move_length{ 13 };

    static bool traffic_auto_flag{ true };
    static bool traffic_stop_flag{ false };

    int fps{ 500 };

    switch (iMessage) {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &clientRect);
        SetTimer(hWnd, 1, fps, NULL);
        SetTimer(hWnd, 2, fps/20, NULL);

        int centerX{ (clientRect.left + clientRect.right) / 2 };
        int centerY{ (clientRect.top + clientRect.bottom) / 2 };

        player = { centerX - 150 - 100, centerY - 150 - 100, centerX - 150, centerY - 150 };

        break;
    }
    case WM_TIMER:
        switch (wParam) {
        case 1:
        {
            if(traffic_auto_flag){
                light_delay++;

                if (keep_time[traffic_light] == light_delay) {
                    traffic_light++;
                    light_delay = 0;
                    player_move_flag = true;
                    if (traffic_light == 4)
                        traffic_light%=4;
                }
                break;
            }
        }
        case 2:
            Car_Move(car, traffic_light, clientRect, move_length, traffic_stop_flag, player_p, player_move, player_move_flag);
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_CHAR:
        switch (wParam) {
        case '+':
            move_length++;
            break;
        case '-':
            if(move_length>1)
                move_length--;
            break;
        case 'a':
        case 'A':
            traffic_auto_flag = true;
            break;
        case 'q':
        case 'Q':
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            PostQuitMessage(0);
            return 0;
        }
        break;
    case WM_LBUTTONDOWN:
        if (not player_move_flag) {
            traffic_stop_flag = true;
            traffic_auto_flag = false;
            player_move_flag = true;
        }
        break;
    case WM_RBUTTONDOWN:
        if(not player_move_flag)
            traffic_stop_flag = false;
        break;
    case WM_PAINT:
        GetClientRect(hWnd, &clientRect);
        hdc = BeginPaint(hWnd, &ps);
        mdc = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
        SelectObject(mdc, (HBITMAP)hBitmap);
        FillRect(mdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        GRID_PAINT(mdc, clientRect, car, traffic_light, player_p, player);
        if (traffic_stop_flag)
            TextOut(mdc, 10, 100, TEXT("traffic_stop_flag"), lstrlen(TEXT("traffic_stop_flag")));
        if(traffic_auto_flag)
            TextOut(mdc, 10, 125, TEXT("traffic_auto_flag"), lstrlen(TEXT("traffic_auto_flag")));

        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, mdc, 0, 0, SRCCOPY);

        DeleteDC(mdc);
        DeleteObject(hBitmap);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
