#pragma once
#include <Windows.h>
#include <vector>
#include "Node.h"
#include "Player.h"
#include "Shape_Paint.h"

void DrawBoard(HDC hdc, const std::vector<Node>& board, Player& p1, Player& p2);

void DrawBoard(HDC hdc, const std::vector<Node>& board, RECT full_rect, Player& p1, Player& p2) {

    //보드판 선
    {
        HBRUSH hbrush = CreateSolidBrush(RGB(200, 200, 255));
        HPEN hpen = CreatePen(PS_SOLID, 1, RGB(200, 200, 255));
        HPEN oldpen = (HPEN)SelectObject(hdc, hpen);
        FrameRect(hdc, &full_rect, hbrush);

        MoveToEx(hdc, full_rect.left, full_rect.top, NULL);
        LineTo(hdc, full_rect.right, full_rect.bottom);

        MoveToEx(hdc, full_rect.right, full_rect.top, NULL);
        LineTo(hdc, full_rect.left, full_rect.bottom);

        SelectObject(hdc, oldpen);
        DeleteObject(hpen);
        DeleteObject(hbrush);
    }

    //보드판 칸
    for (int i{}; i < board.size(); i++) {
        const auto& node = board[i];

        if(i==0 or i==5 or i==10 or i==15 or i==20){
            Rectangle(hdc,
                node.x - 45, node.y - 45,  // 왼쪽 위
                node.x + 45, node.y + 45); // 오른쪽 아래
        }
        else {
            Rectangle(hdc,
                node.x - 30, node.y - 30,  // 왼쪽 위
                node.x + 30, node.y + 30); // 오른쪽 아래
        }
    }

    RECT rect1{ 
        board[p1.pos].x-20,  board[p1.pos].y-20,
        board[p1.pos].x+20,  board[p1.pos].y+20
    };
    HBRUSH hbrush = CreateSolidBrush(p1.color);
    HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
    Circle_Paint(hdc, rect1);
    SelectObject(hdc, oldbrush);
    DeleteObject(hbrush);

    RECT rect2{
        board[p2.pos].x - 20,  board[p2.pos].y - 20,
        board[p2.pos].x + 20,  board[p2.pos].y + 20
    };
    hbrush = CreateSolidBrush(p2.color);
    oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
    Circle_Paint(hdc, rect2);
    SelectObject(hdc, oldbrush);
    DeleteObject(hbrush);
}