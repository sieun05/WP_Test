#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include "Shape.h"
#include "Shape_PAINT.h"

std::random_device rd;  // 시드 생성기
std::mt19937 gen(rd()); // Mersenne Twister 엔진
std::uniform_int_distribution<> dis(0, 5); // 0 ~ 4


void Shape_Move(int dx, int dy, std::vector<Shape>& shapes, int select_index);
void Shape_Create(std::wstring& memo, std::vector<Shape>& shapes, int& select_index);

void Shape_Move(int dx, int dy, std::vector<Shape>& shapes, int select_index){
	if (select_index < 0)
		return;

	Shape& s = shapes[select_index];
	
	int newX1 = s.x1 + dx;
	int newX2 = s.x2 + dx;
	int newY1 = s.y1 + dy;
	int newY2 = s.y2 + dy;


	s.x1 = newX1;
	s.x2 = newX2;
	s.y1 = newY1;
	s.y2 = newY2;
}

void Shape_Create(std::wstring& memo, std::vector<Shape>& shapes, int& select_index) {
	std::wstringstream ss(memo);
	int shape_sel, x1, y1, x2, y2, width, pen_color, brush_color;
	
	ss >> shape_sel >> x1 >> y1 >> x2 >> y2 >> width;

	if (ss.fail()) {
		MessageBox(NULL, TEXT("입력 형식 오류! 올바른 형식: 도형종류 x1 y1 x2 y2 width"), TEXT("Error"), MB_OK);
	}
	else if (shape_sel < 2 || shape_sel > 6) {
		MessageBox(NULL, TEXT("도형 종류는 2~6 범위로 입력해야 합니다."), TEXT("Error"), MB_OK);
	}
	else {
		int pen_color = dis(gen);
		int brush_color = dis(gen);

		shapes.emplace_back(shape_sel, x1, y1, x2, y2, width, pen_color, brush_color);
		select_index = shapes.size() - 1;
	}
	memo.clear();
}