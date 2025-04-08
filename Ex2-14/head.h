#pragma once
#include <Windows.h> 
#include <vector>
#include <random>
#include "GRID_PAINT.h"
#include "Shape.h"

using namespace std;

std::random_device rd;
std::mt19937 dre(rd());
std::uniform_int_distribution<> column(0, COLUMN-1);
std::uniform_int_distribution<> row(0, ROW-1);
std::uniform_int_distribution<> color(0, 255);

using namespace std;

void Shape_Paint(int shape_sel, std::vector<Shape>& shapes, int board[ROW][COLUMN]);
void Shape_Delete(int delete_index, std::vector<Shape>& shapes, int board[ROW][COLUMN]);
void Shape_Move(int dx, int dy, std::vector<Shape>& shapes, int board[ROW][COLUMN]);

void Shape_Paint(int shape_sel, std::vector<Shape>& shapes, int board[ROW][COLUMN]) {
	if (shapes.size() > 9)
		Shape_Delete(0, shapes, board);

	while(true){
		int r_x{ column(dre) }, r_y{ row(dre) };

		if (not board[r_y][r_x]) {
			shapes.emplace_back(r_x, r_y, shape_sel, color(dre), color(dre), color(dre));
			board[r_y][r_x] = (int)shapes.size();
			break;
		}
	}
}

void Shape_Delete(int delete_index, std::vector<Shape>& shapes, int board[ROW][COLUMN]) {
	const Shape& s = shapes[delete_index];
	board[s.y][s.x] = 0;

	shapes.erase(shapes.begin() + delete_index);

	for (int y{}; y < ROW; y++) {
		for (int x{}; x < COLUMN; x++) {
			if (board[y][x] > delete_index + 1) {
				board[y][x] -= 1;
			}
		}
	}
}

void Shape_Move(int dx, int dy, int select_index, std::vector<Shape>& shapes, int board[ROW][COLUMN]) {
	Shape& s = shapes[select_index];
	
	int newX = s.x + dx;
	int newY = s.y + dy;

	if (newX < 0) {
		newX = 39;
	}
	else if (newX > 39) {
		newX %= 40;
	}
	else if(newY<0){
		newY = 39;
	}
	else if (newY > 39) {
		newY %= 40;
	}

	board[newY][newX] = board[s.y][s.x];
	board[s.y][s.x] = 0;

	s.x = newX;
	s.y = newY;


}