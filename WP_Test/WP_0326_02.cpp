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
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //윈도우창의 배경색상 지정 //CreateSolidBrush(RGB(255,0,0));
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = frm_title;

    RegisterClass(&WndClass);

    hWnd = CreateWindow(
        frm_title, //윈도우창 클래스 이름
        TEXT("windows program 2"), //윈도우의 타이틀 이름 (제목표시줄 문자열)
        (WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_BORDER),//윈도우창의 형태지정 //(WS_CAPTION|WS_SYSMENU|WS_MAXIMIZEBOX|WS_THICKFRAME)
        CW_USEDEFAULT, //가로 위치
        CW_USEDEFAULT, //세로 위치
        800,//CW_USEDEFAULT, //창의 폭
        600,//CW_USEDEFAULT, //창의 높이
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
    srand((unsigned int)time(NULL));

    PAINTSTRUCT ps;
    HDC hdc;

    static int x{ 400 }, y{ 300 };
    static int r{ rand() % 255 }, g{ rand() % 255 }, b{ rand() % 255 };
    static TCHAR prt_txt[10][80]{}; //문자열을 저장할 배열
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
