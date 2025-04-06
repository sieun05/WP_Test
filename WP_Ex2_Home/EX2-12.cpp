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
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������â�� ������ ���� //CreateSolidBrush(RGB(255,0,0));
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = frm_title;

    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        frm_title, //������â Ŭ���� �̸�
        TEXT("Ex2-7"), //�������� Ÿ��Ʋ �̸� (����ǥ���� ���ڿ�)
        (WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_BORDER),//������â�� �������� //(WS_CAPTION|WS_SYSMENU|WS_MAXIMIZEBOX|WS_THICKFRAME)
        CW_USEDEFAULT, //���� ��ġ
        CW_USEDEFAULT, //���� ��ġ
        1280,//CW_USEDEFAULT, //â�� ��
        720,//CW_USEDEFAULT, //â�� ����
        NULL, //�θ� ������â�� �ڵ��� ����
        (HMENU)NULL, //������â���� ����� �޴��� �ڵ��� ����
        hInstance, //���α׷��� �ڵ��� ����
        NULL //����ü�� ������ ����
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
    PAINTSTRUCT ps;
    HDC hdc;

    HBRUSH hBrush;
    HBRUSH old_brush;

    HPEN hPen;
    HPEN old_pen;

    static int board[ROW][COLUMN]{0, };

    switch (iMsg) {
    case WM_CREATE:


        break;
    case WM_CHAR:
    {
        switch (wParam) {
        case 't':
            board[5][9] = 1;
            board[8][5] = 1;
            board[0][2] = 1;
            board[7][7] = 1;
            board[7][0] = 1;
            board[9][9] = 1;

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
            break;
        case VK_RIGHT:
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        RECT full_rect{client_rect.left, client_rect.top, client_rect.left+ client_rect.bottom-10, client_rect.bottom-10};
        OffsetRect(&full_rect, 5, 5);

        GRID_PAINT(hdc, full_rect, board); //�׸��� �׸���



        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

