#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define ROW 15
#define COLUMN 15
#define RADIUS 20
#define MAX_FEED 20
#define MAX_OBSTACLE 20

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct Circle {
    int x, y;
    int dx = 1, dy = 0;
    int radius = RADIUS;
    COLORREF color;
    bool isTail = false;
    bool grow = false;
};

struct Feed {
    int x, y;
    COLORREF color;
};

struct Obstacle {
    int x, y;
};

std::vector<Circle> tails;
std::vector<Feed> feeds;
std::vector<Obstacle> obstacles;
Circle head;
bool moveRight = true, moveDown = false, running = false;
int speed = 300;
bool big = false;

void InitGame() {
    srand((UINT)time(nullptr));
    head = { rand() % COLUMN, rand() % ROW, 1, 0, RADIUS, RGB(255, 0, 0), false };
    tails.clear();
    feeds.clear();
    obstacles.clear();

    for (int i = 0; i < MAX_FEED; i++) {
        Feed f = { rand() % COLUMN, rand() % ROW,
                   RGB(rand() % 256, rand() % 256, rand() % 256) };
        feeds.push_back(f);
    }
}

void DrawCircle(HDC hdc, int x, int y, int r, COLORREF color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, x * 40 + 10 - r, y * 40 + 10 - r, x * 40 + 10 + r, y * 40 + 10 + r);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void DrawObstacle(HDC hdc, int x, int y) {
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, x * 40, y * 40, x * 40 + 40, y * 40 + 40);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

void MoveHead() {
    if (!running) return;
    head.x += head.dx;
    head.y += head.dy;

    // 지그재그 반전 조건
    if (head.x < 0) { head.x = 0; head.dx *= -1; head.y++; }
    if (head.x >= COLUMN) { head.x = COLUMN - 1; head.dx *= -1; head.y++; }
    if (head.y < 0) { head.y = 0; head.dy *= -1; head.x++; }
    if (head.y >= ROW) { head.y = ROW - 1; head.dy *= -1; head.x++; }

    // 장애물 충돌 처리
    for (auto& o : obstacles) {
        if (head.x == o.x && head.y == o.y) {
            head.dx *= -1;
            head.dy *= -1;
        }
    }

    // 먹이 먹기
    for (int i = 0; i < feeds.size(); i++) {
        if (head.x == feeds[i].x && head.y == feeds[i].y) {
            Circle tail = { head.x, head.y, (rand() % 3) - 1, (rand() % 3) - 1, RADIUS - 5, feeds[i].color, true };
            tails.push_back(tail);
            feeds.erase(feeds.begin() + i);
            break;
        }
    }

    // 꼬리 따라가기
    for (int i = tails.size() - 1; i > 0; i--) {
        tails[i].x = tails[i - 1].x;
        tails[i].y = tails[i - 1].y;
    }
    if (!tails.empty()) {
        tails[0].x = head.x;
        tails[0].y = head.y;
    }
}

void DrawAll(HDC hdc) {
    for (auto& o : obstacles) DrawObstacle(hdc, o.x, o.y);
    for (auto& f : feeds) DrawCircle(hdc, f.x, f.y, 8, f.color);
    for (auto& t : tails) DrawCircle(hdc, t.x, t.y, t.radius, t.color);
    DrawCircle(hdc, head.x, head.y, head.radius, head.color);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = TEXT("TailGame");
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(TEXT("TailGame"), TEXT("실습 3-2 꼬리 달린 원 게임"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 640, 640, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    SetTimer(hWnd, 1, speed, NULL);
    InitGame();

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_TIMER:
        MoveHead();
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawAll(hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case 'S': running = true; break;
        case VK_LEFT: head.dx = -1; head.dy = 0; break;
        case VK_RIGHT: head.dx = 1; head.dy = 0; break;
        case VK_UP: head.dx = 0; head.dy = -1; break;
        case VK_DOWN: head.dx = 0; head.dy = 1; break;
        case '+': speed = max(50, speed - 50); SetTimer(hWnd, 1, speed, NULL); break;
        case '-': speed += 50; SetTimer(hWnd, 1, speed, NULL); break;
        case 'Q': PostQuitMessage(0); break;
        }
        break;
    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam) / 40;
        int y = HIWORD(lParam) / 40;
        head.x = x;
        head.y = y;
        break;
    }
    case WM_RBUTTONDOWN:
        if (obstacles.size() < MAX_OBSTACLE) {
            int x = LOWORD(lParam) / 40;
            int y = HIWORD(lParam) / 40;
            obstacles.push_back({ x, y });
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}