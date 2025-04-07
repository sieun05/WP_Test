#include <windows.h>
#include <time.h>

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
        800,//CW_USEDEFAULT, //â�� ��
        600,//CW_USEDEFAULT, //â�� ����
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

    static int sel_obj{};

    switch (iMsg) {
    case WM_CREATE:
       

        break;
    case WM_CHAR:
    {
        switch (wParam) {
        case 'l':
            sel_obj = 1;
            break;
        case 't':
            sel_obj = 2;
            break;
        case 'r':
            sel_obj = 3;
            break;
        case 'q':
            PostQuitMessage(0);
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        RECT rect{ client_rect.right / 2. -150 , client_rect.bottom / 2.-150,  client_rect.right / 2. +150,  client_rect.bottom / 2. +150 }; //200 -25        
        RECT rect_2;

        FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH) /*CreateSolidBrush(RGB(220, 220, 220))*/);
        FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //�߰�

        switch(sel_obj){
        case 1:
        {
            rect_2 = rect;
            OffsetRect(&rect_2, -145, -145);
            InflateRect(&rect_2, -145, -145);

            hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
            old_pen = (HPEN)SelectObject(hdc, hPen);

            while (rect.bottom > rect_2.bottom) {
                POINT point_t[4]{ {rect_2.left, rect_2.top}, {rect_2.left, rect_2.bottom} };
                Polygon(hdc, point_t, 2);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, 300, -300);
            while (rect.bottom > rect_2.bottom) {
                POINT point_t[4]{ {rect_2.left, rect_2.top}, {rect_2.left, rect_2.bottom} };
                Polygon(hdc, point_t, 2);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, -300, -300);
            while (rect.right > rect_2.right) {
                POINT point_t[4]{ {rect_2.left, rect_2.top}, {rect_2.right, rect_2.top} };
                Polygon(hdc, point_t, 2);
                OffsetRect(&rect_2, 15, 0);
            }

            OffsetRect(&rect_2, -300, +300);
            while (rect.right > rect_2.right) {
                POINT point_t[4]{ {rect_2.left, rect_2.top}, {rect_2.right, rect_2.top} };
                Polygon(hdc, point_t, 2);
                OffsetRect(&rect_2, 15, 0);
            }

            DeleteObject(hPen);
        }
            break;
        case 2:
        {
            rect_2 = rect;
            OffsetRect(&rect_2, -145 - 5, -145);
            InflateRect(&rect_2, -145, -145);

            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            old_brush = (HBRUSH)SelectObject(hdc, hBrush);

            while (rect.bottom > rect_2.bottom) {
                POINT point_t[6]{ {(rect_2.left + rect_2.right) / 2., rect_2.top} , {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.bottom} };
                Polygon(hdc, point_t, 3);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, 300, -300);
            while (rect.bottom > rect_2.bottom) {
                POINT point_t[6]{ {(rect_2.left + rect_2.right) / 2., rect_2.top} , {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.bottom} };
                Polygon(hdc, point_t, 3);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, -300 + 15, -300 - 5);
            while (rect.right - 5 > rect_2.right) {
                POINT point_t[6]{ {(rect_2.left + rect_2.right) / 2., rect_2.top} , {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.bottom} };
                Polygon(hdc, point_t, 3);
                OffsetRect(&rect_2, 15, 0);
            }

            OffsetRect(&rect_2, -300 + 15, +300);
            while (rect.right - 5 > rect_2.right) {
                POINT point_t[6]{ {(rect_2.left + rect_2.right) / 2., rect_2.top} , {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.bottom} };
                Polygon(hdc, point_t, 3);
                OffsetRect(&rect_2, 15, 0);

                DeleteObject(hBrush);
            }
        }
            break;
        case 3:
        {
            rect_2 = rect;
            OffsetRect(&rect_2, -145 - 5, -145);
            InflateRect(&rect_2, -145, -145);

            hBrush = CreateSolidBrush(RGB(0, 255, 0));
            old_brush = (HBRUSH)SelectObject(hdc, hBrush);

            while (rect.bottom > rect_2.bottom) {
                FillRect(hdc, &rect_2, hBrush);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, 300, -300);
            while (rect.bottom > rect_2.bottom) {
                FillRect(hdc, &rect_2, hBrush);
                OffsetRect(&rect_2, 0, 15);
            }

            OffsetRect(&rect_2, -300 + 15, -300 - 5);
            while (rect.right - 5 > rect_2.right) {
                FillRect(hdc, &rect_2, hBrush);
                OffsetRect(&rect_2, 15, 0);
            }

            OffsetRect(&rect_2, -300 + 15, +300);
            while (rect.right - 5 > rect_2.right) {
                FillRect(hdc, &rect_2, hBrush);
                OffsetRect(&rect_2, 15, 0);

                DeleteObject(hBrush);
            }
        }
        break;
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
