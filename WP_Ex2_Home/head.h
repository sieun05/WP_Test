#pragma once
#include <windows.h>
#include <time.h>

struct Player {
	Player() = default;
	Player(int x, int y) : x{ x }, y{ y } {}
    Player(int x, int y, int size, int r, int g, int b) : x{ x }, y{ y }, size(size), r{ r }, g{ g }, b{ b } {}

    friend bool operator==(const Player& a, const Player& b) {
        return a.x == b.x &&
            a.y == b.y &&
            a.size == b.size &&
            a.shape == b.shape &&
            a.r == b.r &&
            a.g == b.g &&
            a.b == b.b;
    }

	int x{}, y{};
	int size{ -2 };
	int shape{}; //0 (±âº» ¿ø), 1 (»ï°¢Çü), 2 (»çÀÛÇü), 3(¸¶¸§¸ð)
	int r{255}, g{255}, b{255}; //0 (»¡°­), 1 (ÆÄ¶û), 2 (ÃÊ·Ï), 3 (³ë¶û)
};

void UpdatePlayerState(Player& p, int cell_value) {
    switch (cell_value) {
    case 4:
        p.r = 60, p.g = 250, p.b = 250;
        break;
    case 5:
        p.r = 200, p.g = 0, p.b = 200;
        break;
    case 6:
        p.r = 20, p.g = 200, p.b = 200;
        break;
    case 7:
        p.shape = (p.shape + 1) % 4;
        break;
    case 8:
        if (p.size > -5) p.size--;
        break;
    case 9:
        if (p.size < 0) p.size++;
        break;
    }
}