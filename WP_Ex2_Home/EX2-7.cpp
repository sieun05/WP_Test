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

//void Txt_right_left;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static TCHAR memo_txt[10][30]{};
    static int caret_x{}, caret_y{};
    
    static int f1_flag{};
    static int insert_flag{};
    static int insert_flag_2{};

    static TCHAR prt_txt[100]{};

    switch (iMsg) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        SetCaretBlinkTime(500);
        ShowCaret(hWnd);
        
        break;
    case WM_CHAR:
    {
        switch (wParam) {
        case VK_RETURN:
        {
            if (caret_y < 9) {
                caret_y++;
                if (caret_x >= lstrlen(memo_txt[caret_y])) {
                    caret_x = lstrlen(memo_txt[caret_y]);
                }
            }
        }
        break;
        case VK_BACK:
        {
            if (caret_x > 0) {
                for (int i{ caret_x }; i < lstrlen(memo_txt[caret_y]) - 1; i++) {
                    memo_txt[caret_y][i] = memo_txt[caret_y][i + 1];
                }
                memo_txt[caret_y][lstrlen(memo_txt[caret_y]) - 1] = '\0';
                caret_x--;
            }
            else if (caret_y > 0) {
                caret_y--;
                caret_x = lstrlen(memo_txt[caret_y]);
            }
        }
        break;
        case VK_ESCAPE:
        {
            for (int i{}; i < 10; i++) {
                memset(memo_txt[i], 0, sizeof(memo_txt[i]));
            }
            caret_x = 0;
            caret_y = 0;

            break;
        }
        case VK_TAB:
        {
            for (int i{}; i < 5; i++) {
                if (lstrlen(memo_txt[caret_y]) < 30 - 1) {
                    for (int j{ lstrlen(memo_txt[caret_y]) }; j > caret_x; j--) {
                        memo_txt[caret_y][j] = memo_txt[caret_y][j - 1];
                    }
                    memo_txt[caret_y][caret_x++] = ' ';
                }
                memo_txt[caret_y][lstrlen(memo_txt[caret_y])] = '\0';
            }
            break;
        }
        default:
            if (lstrlen(memo_txt[caret_y]) >= 30 - 1) {
                if (caret_x < lstrlen(memo_txt[caret_y])) {
                    insert_flag_2 = 1;
                }
                else if (caret_y > 8) {
                    caret_x = 0;
                    caret_y = 0;
                    insert_flag_2 = 1;
                }
                else {
                    caret_y++;
                    caret_x = 0;
                    insert_flag_2 = 0;
                }
            }

            memo_txt[caret_y][lstrlen(memo_txt[caret_y])] = '\0';
            if(not insert_flag and not insert_flag_2){
                for (int i{ lstrlen(memo_txt[caret_y])}; i > caret_x; i--) {
                    memo_txt[caret_y][i] = memo_txt[caret_y][i - 1];
                }
                memset(memo_txt[caret_y] + lstrlen(memo_txt[caret_y]), 0, 30 - lstrlen(memo_txt[caret_y]));
            }
            if(f1_flag){
                memo_txt[caret_y][caret_x++] = toupper(wParam);
            }
            else {
                memo_txt[caret_y][caret_x++] = wParam;
            }
               
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
        {
            if (caret_x > 0) {
                caret_x--;
            }
            else if (caret_y > 0) {
                caret_y--;
                caret_x = lstrlen(memo_txt[caret_y]);
            }
            break;
        }
        case VK_RIGHT:
        {
            if (caret_x < sizeof(memo_txt[caret_y]) and caret_x < lstrlen(memo_txt[caret_y])) {
                caret_x++;
            }
            else if (caret_y < 9) {
                caret_y++;
                caret_x = 0;
            }
            break;
        }
        case VK_UP:
        {
            if (caret_y > 0) {
                caret_y--;
                if (caret_x > lstrlen(memo_txt[caret_y])) {
                    caret_x = lstrlen(memo_txt[caret_y]);
                }
            }
            break;
        }
        case VK_DOWN:
        {
            if (caret_y < 9) {
                caret_y++;
                if (caret_x > lstrlen(memo_txt[caret_y])) {
                    caret_x = lstrlen(memo_txt[caret_y]);
                }
            }
            break;

        }
<<<<<<< HEAD
        
=======
>>>>>>> b867915dc7396f2b4b3af742b12b0e29d5297b6e
        case VK_F1:
            f1_flag = f1_flag ? 0 : 1;
            break;
        case VK_F2:

            break;
        case VK_F3:
            break;
        case VK_F4:
            break;
        case VK_DELETE:
        {
            int len = lstrlen(memo_txt[caret_y]);
            if (len == 0 || caret_x >= len) break;

            // 1. �ܾ� ���۰� �� ã��
            int word_start = caret_x;
            while (word_start > 0 && memo_txt[caret_y][word_start - 1] != ' ')
                word_start--;

            int word_end = caret_x;
            while (word_end < len && memo_txt[caret_y][word_end] != ' ')
                word_end++;

            int word_length = word_end - word_start;

            // 2. �ܾ� ���� (shift left)
            for (int i = word_end; i <= len; i++) {
                memo_txt[caret_y][i - word_length] = memo_txt[caret_y][i];
                memo_txt[caret_y][lstrlen(memo_txt[caret_y])] = '\0';
            }

            // 3. ĳ�� ��ġ ����
            if (word_start > 0) {
                caret_x = word_start - 1; // ���� �� (�� �ܾ� ��)
                //memset(memo_txt[caret_y] + lstrlen(memo_txt[caret_y]), 0, 30 - lstrlen(memo_txt[caret_y]));
            }
            else{
                caret_x = 0; // �� �� ��
                memset(memo_txt[caret_y], 0, 30);
            }

            /*int word_start{}, word_end{};
            for (int i{ caret_x }; i >= 0; i--) {
                if (memo_txt[caret_y][i] == ' ') {
                    word_start = i;
                }
            }
            for (int i{ caret_x }; i < lstrlen(memo_txt[caret_y]); i++) {
                if (memo_txt[caret_y][i] == ' ') {
                    word_end = i;
                }
            }

            for (int i{}; i < word_end - word_start; i++) {
                for (int j{ word_start }; j < lstrlen(memo_txt[caret_y]) - 1; j++) {
                    memo_txt[caret_y][j] = memo_txt[caret_y][j + 1];
                }
                memo_txt[caret_y][lstrlen(memo_txt[caret_y]) - 1] = '\0';
            }

            caret_x = lstrlen(memo_txt[caret_y]);*/
            break;
        }
        case VK_HOME:
        {
            caret_x = 0;
            insert_flag = 1;
            break;
        }
        case VK_END:
        {
            if (lstrlen(memo_txt[caret_y]) < 30 - 1) {
                caret_x = lstrlen(memo_txt[caret_y]);
            }
            else {
                caret_y++;
                caret_x = 0;
            }
        }
        break;
        case VK_INSERT:
        {
            insert_flag = insert_flag ? 0 : 1;
            break;
        }
        case VK_PRIOR:
            if (caret_y - 3 > 0) {
                caret_y -= 3;
            }
            else {
                caret_y = 0;
            }
            if (caret_x > lstrlen(memo_txt[caret_y])) {
                caret_x = lstrlen(memo_txt[caret_y]);
            }
            break;
        case VK_NEXT:
            if (caret_y + 3 < 9) {
                caret_y += 3;
            }
            else {
                caret_y = 9;
            }
            if (caret_x > lstrlen(memo_txt[caret_y])) {
                caret_x = lstrlen(memo_txt[caret_y]);
            }
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        SIZE size;
        GetTextExtentPoint32(hdc, memo_txt[caret_y], caret_x, &size);
        for (int i{}; i < 10; i++) {
            TextOut(hdc, 10, 10 + 25*i, memo_txt[i], lstrlen(memo_txt[i]));
        }
        SetCaretPos(10 + size.cx, 10 + 25 * caret_y);

        TextOut(hdc, 0, 400, prt_txt, lstrlen(prt_txt));

        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        DestroyCaret();
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}
