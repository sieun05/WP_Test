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
    srand((unsigned)time(NULL));

    PAINTSTRUCT ps;
    HDC hdc;

    HBRUSH hBrush{};
    HBRUSH old_brush{};

    HPEN hPen;
    HPEN old_pen;

    static int sel_obj{};
    static int r{}, g{}, b{};

    static bool key_pressed[256] = { false };
    static COLORREF defaultColor = RGB(255, 255, 255);
    static COLORREF pressedColor{};

    static int rotate{};
    static int shape_positions[4] = { 1, 2, 3, 4 };

    switch (iMsg) {
    case WM_CREATE:
        //SetTimer(hWnd, 1, 50, NULL);

        break;
    //case WM_TIMER:
    //    switch (wParam) {
    //    case 1:
    //        if (GetKeyState('c') & 0x8000) {
    //            key_down = 1;
    //        }
    //        else {
    //            key_down = 0;
    //        }
    //    }
    //    break;
    case WM_CHAR:
    {
        key_pressed[wParam] = true;

        switch (wParam) {
        case 'c':   //�� ����
            sel_obj = 1;
            break;
        case 's': //�𷡽ð� ����
            sel_obj = 2;
            break;
        case 'p':   //������ ����
            sel_obj = 3;
            break;
        case 'e':   //���̸�� ����
            sel_obj = 4;
            break;
        case 'q':
        case 'Q':
            PostQuitMessage(0);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYDOWN:
        if (wParam < 256) key_pressed[tolower(wParam)] = true;

        switch (wParam) {;
        case VK_LEFT:
        {
            int temp = shape_positions[0];
            for (int i = 0; i < 3; ++i)
                shape_positions[i] = shape_positions[i + 1];
            shape_positions[3] = temp;
            sel_obj = shape_positions[1];
            break;
        }
        case VK_RIGHT:
        {
            int temp = shape_positions[3];
            for (int i = 3; i > 0; --i)
                shape_positions[i] = shape_positions[i - 1];
            shape_positions[0] = temp;
            sel_obj = shape_positions[1];
            break;
        }
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYUP:
        if (wParam < 256) key_pressed[tolower(wParam)] = false;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);

        RECT client_rect;
        GetClientRect(hWnd, &client_rect);

        RECT rect{ client_rect.right / 2. - 75 , client_rect.bottom / 2. - 75,  client_rect.right / 2. + 75,  client_rect.bottom / 2. + 75 }; //200 -25

        hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        old_pen = (HPEN)SelectObject(hdc, hPen);

        FillRect(hdc, &rect, CreateSolidBrush(RGB(220, 220, 220)));
        FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //�߰�
        RECT rect_2{ rect };
        InflateRect(&rect_2, -20, -20);

        bool isHeld = key_pressed['c'] || key_pressed['s'] || key_pressed['p'] || key_pressed['e'];
        if (isHeld) {
            r = rand() % 255; g = rand() % 255; b = rand() % 255;
            pressedColor = RGB(r, g, b);
        }
        else {
            pressedColor = defaultColor;
        }

        HBRUSH hBrush = CreateSolidBrush(pressedColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        switch (sel_obj) {
        case 1:
        {
            Ellipse(hdc, rect_2.left, rect_2.top, rect_2.right, rect_2.bottom);
            break;
        }
        case 2:
        {
            POINT point[8]{ {rect_2.left, rect_2.top}, {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.top}, {rect_2.right, rect_2.bottom} };
            Polygon(hdc, point, 4);
            break;
        }
        case 3:
        {
            POINT point_p[10]{ {rect_2.left + 30, rect_2.top}, {rect_2.right - 30, rect_2.top}, {rect_2.right ,(rect_2.top + rect_2.bottom) / 2. + 10}, {(rect_2.left + rect_2.right) / 2., rect_2.bottom},    {rect_2.left ,(rect_2.top + rect_2.bottom) / 2. + 10} };
            Polygon(hdc, point_p, 5);
            break;
        }
        case 4:
        {
            Pie(hdc, rect_2.left, rect_2.top, rect_2.right, rect_2.bottom, rect_2.right, (rect_2.top + rect_2.bottom) / 2., (rect_2.left + rect_2.right) / 2., rect_2.top);
            break;
        }
        }

        SelectObject(hdc, old_brush);
        DeleteObject(hBrush);

        int dx[4] = { 0, 160, 0, -160 };
        int dy[4] = { -160, 0, 160, 0 };

        for (int i = 0; i < 4; ++i) {
            RECT r = rect;
            OffsetRect(&r, dx[i], dy[i]);
            RECT r2 = r;
            InflateRect(&r2, -20, -20);

            // ���� ���� ����
            //COLORREF color;
            //switch (i) {
            //case 0: color = RGB(255, 0, 0); break; // ����
            //case 1: color = RGB(255, 255, 0); break; // ������
            //case 2: color = RGB(0, 255, 0); break; // �Ʒ���
            //case 3: color = RGB(15, 15, 255); break; // ����
            //}

            

            switch (shape_positions[i]) {
            case 1: // ��
                hBrush = CreateSolidBrush(RGB(0, 0, 255));
                old_brush = (HBRUSH)SelectObject(hdc, hBrush);
                
                Ellipse(hdc, r2.left, r2.top, r2.right, r2.bottom);

                SelectObject(hdc, old_brush);
                DeleteObject(hBrush);
                break;
            case 2: // �𷡽ð� (�簢�� 2��)
            {
                hBrush = CreateSolidBrush(RGB(255, 0, 0));
                old_brush = (HBRUSH)SelectObject(hdc, hBrush);

                POINT point_tt[8]{ {r2.left, r2.top}, {r2.right, r2.top}, {r2.left, r2.bottom}, {r2.right, r2.bottom} };
                Polygon(hdc, point_tt, 4);

                SelectObject(hdc, old_brush);
                DeleteObject(hBrush);
                break;
            }
            case 3: // ������
            {
                hBrush = CreateSolidBrush(RGB(255, 255, 0));
                old_brush = (HBRUSH)SelectObject(hdc, hBrush);

                POINT point_p[10]{ 
                    {(r2.left + r2.right) / 2., r2.top}, 
                    {r2.right ,(r2.top + r2.bottom) / 2. - 10}, 
                    {r2.right - 30, r2.bottom}, 
                    {r2.left + 30, r2.bottom}, 
                    {r2.left ,(r2.top + r2.bottom) / 2. - 10} };
                Polygon(hdc, point_p, 5);

                SelectObject(hdc, old_brush);
                DeleteObject(hBrush);
                break;
            }
            case 4: // ����
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                old_brush = (HBRUSH)SelectObject(hdc, hBrush);

                Pie(hdc, r2.left, r2.top, r2.right, r2.bottom, 
                    (r2.left + r2.right) / 2., r2.top, 
                    r2.right, (r2.top + r2.bottom) / 2.);

                SelectObject(hdc, old_brush);
                DeleteObject(hBrush);
                break;
            }
            SelectObject(hdc, old_brush);
            DeleteObject(hBrush);
        }


        //OffsetRect(&rect, 0, 160);
        ////FillRect(hdc, &rect, CreateSolidBrush(RGB(220, 220, 220)));
        ////FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //�Ʒ�

        //hBrush = CreateSolidBrush(RGB(0, 255, 0));
        //old_brush = (HBRUSH)SelectObject(hdc, hBrush);

        //rect_2 = rect;
        //InflateRect(&rect_2, -20, -20);
        //Pie(hdc, rect_2.left, rect_2.top, rect_2.right, rect_2.bottom, (rect_2.left + rect_2.right) / 2., rect_2.top, rect_2.right, (rect_2.top + rect_2.bottom) / 2.);

        //SelectObject(hdc, old_brush);
        //DeleteObject(hBrush);

        //OffsetRect(&rect, 0, -320);
        ////FillRect(hdc, &rect, CreateSolidBrush(RGB(220, 220, 220)));
        ////FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //��

        //hBrush = CreateSolidBrush(RGB(255, 0, 0));
        //old_brush = (HBRUSH)SelectObject(hdc, hBrush);

        //rect_2 = rect;
        //InflateRect(&rect_2, -20, -20);
        //POINT point_tt[8]{ {rect_2.left, rect_2.top}, {rect_2.right, rect_2.top}, {rect_2.left, rect_2.bottom}, {rect_2.right, rect_2.bottom} };
        //Polygon(hdc, point_tt, 4);

        //SelectObject(hdc, old_brush);
        //DeleteObject(hBrush);

        //OffsetRect(&rect, 160, 160);
        ////FillRect(hdc, &rect, CreateSolidBrush(RGB(220, 220, 220)));
        ////FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //������

        //hBrush = CreateSolidBrush(RGB(255, 255, 0));
        //old_brush = (HBRUSH)SelectObject(hdc, hBrush);

        //rect_2 = rect;
        //InflateRect(&rect_2, -20, -20);
        //POINT point_p[10]{ {(rect_2.left + rect_2.right) / 2., rect_2.top}, {rect_2.right ,(rect_2.top + rect_2.bottom) / 2. - 10}, {rect_2.right - 30, rect_2.bottom}, {rect_2.left + 30, rect_2.bottom}, {rect_2.left ,(rect_2.top + rect_2.bottom) / 2. - 10} };
        //Polygon(hdc, point_p, 5);

        //SelectObject(hdc, old_brush);
        //DeleteObject(hBrush);

        //OffsetRect(&rect, -320, 0);
        ////FillRect(hdc, &rect, CreateSolidBrush(RGB(220, 220, 220)));
        ////FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH)); //����

        //hBrush = CreateSolidBrush(RGB(15, 15, 255));
        //old_brush = (HBRUSH)SelectObject(hdc, hBrush);

        //rect_2 = rect;
        //InflateRect(&rect_2, -20, -20);
        //Ellipse(hdc, rect_2.left, rect_2.top, rect_2.right, rect_2.bottom);

        //SelectObject(hdc, old_brush);
        //SelectObject(hdc, old_pen);

        /*DeleteObject(hBrush);
        DeleteObject(hPen);*/
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
