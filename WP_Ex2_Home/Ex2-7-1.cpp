#include <windows.h>
#include <string>
#include <cwctype> // for towupper
#include <algorithm> // for std::min

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR frm_title = TEXT("c_form_view");
HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = frm_title;

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(frm_title, TEXT("Ex2-7"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::wstring memo_txt[10];
    static int caret_x = 0, caret_y = 0;
    static bool f1_flag = false;
    static bool f2_flag = false;
    static bool f3_flag = false;
    static bool f4_flag = false;
    static bool f5_flag = false;
    static bool insert_flag = false;

    switch (msg) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;

    case WM_CHAR:
        switch (wParam) {
        case VK_RETURN:
            if (caret_y < 9) {
                caret_y++;
                caret_x = (int)(std::min<size_t>(caret_x, memo_txt[caret_y].length()));
            }
            break;

        case VK_BACK:
            if (caret_x > 0) {
                memo_txt[caret_y].erase(caret_x - 1, 1);
                caret_x--;
            }
            else if (caret_y > 0) {
                caret_y--;
                caret_x = (int)memo_txt[caret_y].length();
            }
            break;

        case VK_ESCAPE:
            for (auto& line : memo_txt) line.clear();
            caret_x = caret_y = 0;
            break;

        case VK_TAB:
            memo_txt[caret_y].insert(caret_x, 5, L' ');
            caret_x += 5;
            break;

        case L'+':
            for (auto& line : memo_txt) {
                std::wstring result;
                for (size_t i = 0; i < line.size(); ) {
                    if (iswdigit(line[i])) {
                        size_t j = i;
                        while (j < line.size() && iswdigit(line[j])) j++;
                        std::wstring number_str = line.substr(i, j - i);
                        int number = std::stoi(number_str);
                        number = (number + 1) % 10000; // wraparound for 4-digit numbers
                        result += std::to_wstring(number);
                        i = j;
                    }
                    else {
                        wchar_t ch = line[i];
                        if (ch >= L'a' && ch <= L'y') ch++;
                        else if (ch == L'z') ch = L'a';
                        else if (ch >= L'A' && ch <= L'Y') ch++;
                        else if (ch == L'Z') ch = L'A';
                        result += ch;
                        i++;
                    }
                }
                line = result;
            }
            break;

        case L'-':
            for (auto& line : memo_txt) {
                std::wstring result;
                for (size_t i = 0; i < line.size(); ) {
                    if (iswdigit(line[i])) {
                        size_t j = i;
                        while (j < line.size() && iswdigit(line[j])) j++;
                        std::wstring number_str = line.substr(i, j - i);
                        int number = std::stoi(number_str);
                        number = (number + 9999) % 10000; // wraparound backward
                        result += std::to_wstring(number);
                        i = j;
                    }
                    else {
                        wchar_t ch = line[i];
                        if (ch >= L'b' && ch <= L'z') ch--;
                        else if (ch == L'a') ch = L'z';
                        else if (ch >= L'B' && ch <= L'Z') ch--;
                        else if (ch == L'A') ch = L'Z';
                        result += ch;
                        i++;
                    }
                }
                line = result;
            }
            break;


        default:
            if (memo_txt[caret_y].length() >= 29) break;
            wchar_t ch = static_cast<wchar_t>(wParam);
            if (f1_flag) ch = towupper(ch);

            if (insert_flag && caret_x < memo_txt[caret_y].size()) {
                memo_txt[caret_y][caret_x] = ch;
            }
            else {
                memo_txt[caret_y].insert(memo_txt[caret_y].begin() + caret_x, ch);
            }
            caret_x++;
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_LEFT:
            if (caret_x > 0) caret_x--;
            else if (caret_y > 0) {
                caret_y--;
                caret_x = (int)memo_txt[caret_y].length();
            }
            break;
        case VK_RIGHT:
            if (caret_x < memo_txt[caret_y].length()) caret_x++;
            else if (caret_y < 9) {
                caret_y++;
                caret_x = 0;
            }
            break;
        case VK_UP:
            if (caret_y > 0) caret_y--;
            caret_x = (int)(std::min<size_t>(caret_x, memo_txt[caret_y].length()));
            break;
        case VK_DOWN:
            if (caret_y < 9) caret_y++;
            caret_x = (int)(std::min<size_t>(caret_x, memo_txt[caret_y].length()));
            break;
        case VK_HOME:
            caret_x = 0;
            break;
        case VK_END:
            caret_x = (int)memo_txt[caret_y].length();
            break;
        case VK_INSERT:
            insert_flag = !insert_flag;
            break;
        case VK_F1:
            f1_flag = !f1_flag;
            break;
        case VK_F2:
            f2_flag = !f2_flag;
            for (int i = 0; i < 10; ++i) {
                bool has_digit = memo_txt[i].find_first_of(L"0123456789") != std::wstring::npos;
                if (has_digit) {
                    if (f2_flag) {
                        memo_txt[i].insert(0, L"****");
                    }
                    else if (memo_txt[i].substr(0, 4) == L"****") {
                        memo_txt[i].erase(0, 4);
                    }
                }
            }
            break;

        case VK_F3:
            f3_flag = !f3_flag;
            for (int i = 0; i < 10; ++i) {
                if (f3_flag) {
                    std::wstring result;
                    std::wstring word;
                    for (wchar_t ch : memo_txt[i]) {
                        if (ch == L' ') {
                            if (!word.empty()) {
                                result += L"(" + word + L") ";
                                word.clear();
                            }
                            else {
                                result += L" ";
                            }
                        }
                        else {
                            word += towupper(ch);
                        }
                    }
                    if (!word.empty()) {
                        result += L"(" + word + L")";
                    }
                    memo_txt[i] = result;
                }
                else {
                    std::wstring restored;
                    bool in_paren = false;
                    for (wchar_t ch : memo_txt[i]) {
                        if (ch == L'(' || ch == L')') continue;
                        if (ch == L' ') restored += L' ';
                        else restored += towlower(ch);
                    }
                    memo_txt[i] = restored;
                }
            }
            break;
        case VK_F4:
        {
            f4_flag = !f4_flag;
            static std::wstring original_with_spaces[10];
            if (f4_flag) {
                for (int i = 0; i < 10; ++i) {
                    original_with_spaces[i] = memo_txt[i];
                    memo_txt[i].erase(std::remove(memo_txt[i].begin(), memo_txt[i].end(), L' '), memo_txt[i].end());
                }
            }
            else {
                for (int i = 0; i < 10; ++i) {
                    memo_txt[i] = original_with_spaces[i];
                }
            }
            break;
        }
        case VK_F5:
        {
            f5_flag = !f5_flag;
            static std::wstring original_f5_text[10];
            static wchar_t f5_target_char = 0;
            if (f5_flag) {
                for (int i = 0; i < 10; ++i) {
                    original_f5_text[i] = memo_txt[i];
                }
                for (int i = 0; i < 10 && f5_target_char == 0; ++i) {
                    for (wchar_t ch : memo_txt[i]) {
                        if (ch != L' ') {
                            f5_target_char = ch;
                            break;
                        }
                    }
                }
                if (f5_target_char != 0) {
                    for (int i = 0; i < 10; ++i) {
                        for (auto& ch : memo_txt[i]) {
                            if (ch == f5_target_char) ch = L'@';
                        }
                    }
                }
            }
            else {
                for (int i = 0; i < 10; ++i) {
                    memo_txt[i] = original_f5_text[i];
                }
                f5_target_char = 0;
            }
            break;
        }
        case VK_F6:
            std::wstring temp = memo_txt[0];
            for (int i = 0; i < 9; ++i) {
                memo_txt[i] = memo_txt[i + 1];
            }
            memo_txt[9] = temp;
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        for (int i = 0; i < 10; ++i) {
            TextOutW(hdc, 10, 10 + 25 * i, memo_txt[i].c_str(), memo_txt[i].length());
        }
        SIZE size;
        GetTextExtentPoint32W(hdc, memo_txt[caret_y].c_str(), caret_x, &size);
        SetCaretPos(10 + size.cx, 10 + 25 * caret_y);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        DestroyCaret();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
