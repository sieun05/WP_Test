#pragma once
#include <windows.h>		//--- ������ ��� ����
#include <tchar.h>
#include <vector>
#include <random>
#include "resource.h"
#include "Cell.h"
#include "GRID_PAINT.h"

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> r_width(0, 1280);
uniform_int_distribution<> r_height(0, 720);

void Cell_Move(std::vector<Cell>& cell, int mx, int my, const RECT& f_rect, const int& camera_mode) {
	POINT mp{ mx, my };
	if (not PtInRect(&f_rect, mp)) return;

	for (int i{}; i < cell.size(); i++) {
		Cell& c = cell.at(i);

		int centerX{ (f_rect.left + f_rect.right) / 2 };
		int centerY{ (f_rect.top + f_rect.bottom) / 2 };

		float dx{};
		float dy{};

		if (camera_mode == 1) {
			/*
			ī�޶� �̵� ����� �� ��ǥ������ ���ϸ� 
			������ ��ǥ���� ���콺�� ��ǥ���� �Ǿ��� �� ���߱⵵ �ϰ�...
			�̻��� �������� �̵��ϱ⵵ �Ѵ�...

			�𼭸� �κп����� ȭ���� �� �����̰� ������ �����̴ϱ�
			�ٽ� ��ǥ������ ���� �� �ְ� �ؾ��� �� ����
			*/
			dx = (float)(mx - centerX);
			dy = (float)(my - centerY);
		}
		else {
			dx = (float)(mx - c.x);
			dy = (float)(my - c.y);
		}

		float length = sqrtf(dx * dx + dy * dy);

		//��������
		float nx = dx / length;
		float ny = dy / length;

		c.fx += nx * c.speed;
		c.fy += ny * c.speed;

		c.x = static_cast<int>(c.fx);
		c.y = static_cast<int>(c.fy);
	}
}