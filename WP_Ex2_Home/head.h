#pragma once
#include <windows.h>
#include <time.h>

class Player {
public:
	Player() = default;
	Player(int x, int y) : p{ {x,y} } {}
	Player(int x, int y, int size) : p{ {x,y} }, size(size) {}

	void setPoint(int x, int y) {
		p[0].x = x;
		p[0].y = y;
	}

	void setSize(int s) {
		size = s;
	}

	void setShape(int s) {
		shape = s;
	}

	void setColor(int c) {
		color = c;
	}

	POINT getPoint() const {
		return p[0];
	}

	int getSize() const {
		return size;
	}

	int getShape() const {
		return shape;
	}

	int getColor() const {
		return color;
	}

private:
	POINT p[2]{};
	int size{ -2 };
	int shape{}; //0 (�⺻ ��), 1 (�ﰢ��), 2 (������), 3(������)
	int color{}; //0 (����), 1 (�Ķ�), 2 (�ʷ�), 3 (���)
};
