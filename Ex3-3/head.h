#pragma once
#include <Windows.h>
#include <random>
#include "Node.h"
#include "PAINT.h"
#include "Player.h"

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, 1);

void MovePlayer(const HWND& hWnd, Player& p, int steps, const std::vector<Node>& board);
int ThrowYut();

void MovePlayer(const HWND& hWnd, Player& p, int steps, const std::vector<Node>& board) {
    if (steps == 0) steps = 5;
    bool fast_way{false};
    if (p.pos == 5 or p.pos == 10 or p.pos == 20) {
        fast_way = true;
    }

	for (int i{}; i < steps; i++) {
		if (p.finished) return;
		if (board[p.pos].next.empty()) {
			p.finished = true;
			break;
		}

        if(fast_way and i==0){
            p.pos = board[p.pos].next[1];
        }
        else {
            p.pos = board[p.pos].next[0];
        }


        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
		Sleep(200);
	}
}

int ThrowYut() {
    int front = 0;
    for (int i = 0; i < 4; ++i) {
        int face{dis(gen)}; // 0: 뒷면, 1: 앞면
        front += face;
    }

    return front;

    //switch (front) {
    //case 0: return 0;
    //case 1: return 1;
    //case 2: return 2;
    //case 3: return 3;
    //case 4: return 4;
    //}
    //return 1; // fallback
}