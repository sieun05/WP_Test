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
    WndClass.hCursor = LoadCursor(NULL, IDI_WARNING);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������â�� ������ ���� //CreateSolidBrush(RGB(255,0,0));
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = frm_title;

    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        frm_title, //������â Ŭ���� �̸�
        TEXT("windows program 2"), //�������� Ÿ��Ʋ �̸� (����ǥ���� ���ڿ�)
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
    srand((unsigned int)time(NULL));

    PAINTSTRUCT ps;
    HDC hdc;

    static int x{ 400 }, y{ 300 };
    static int r{ rand() % 255 }, g{ rand() % 255 }, b{ rand() % 255 };
    static TCHAR prt_txt[10][80]{}; //���ڿ��� ������ �迭
    static int count = 0;
    static int row{};

    switch (iMsg) {
    case WM_CHAR:
        if (wParam == VK_BACK) {
            if (count > 0) {
                count--;
                prt_txt[row][count] = '\0';
            }
        }
        else if (wParam == VK_RETURN) {
            if(row < 9){
                row++;
                count = 0;
            }
        }
        else {
            prt_txt[row][count++] = wParam;
            prt_txt[row][count] = '\0';
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        for (int i{}; i <= row; i++){
            SetTextColor(hdc, RGB(r + 50*i, g + 30 * i, b + 10 * i));
            TextOut(hdc, x-10*i, y+20*i, prt_txt[i], lstrlen(prt_txt[i]));
            TextOut(hdc, x - 10 * i, y - 20 * i, prt_txt[i], lstrlen(prt_txt[i]));
        }

        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
