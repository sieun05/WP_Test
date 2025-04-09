#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "Shape.h"
#include "Shape_PAINT.h"

void Shape_Move(int dx, int dy, std::vector<Shape>& shapes, int select_index);


void Shape_Move(int dx, int dy, std::vector<Shape>& shapes, int select_index){
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