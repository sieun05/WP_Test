#pragma once

struct Block_1 { //세로4칸블럭
	Block_1() = default;
	Block_1(int x, int y, int color) 
		: x{ x }, y{ y }, color{ color } {
		width = 1;
		height = 4;
	}
	~Block_1() = default;
	Block_1(const Block_1& b) = default;
	Block_1& operator=(const Block_1& b) = default;
	
	int x, y;
	int width;
	int height;
	int color;
};

struct Block_2 { //가로4칸블록
	Block_2() = default;
	Block_2(int x, int y, int color)
		: x{ x }, y{ y }, color{ color } {
		width = 4;
		height = 1;
	}
	~Block_2() = default;
	Block_2(const Block_2& b) = default;
	Block_2& operator=(const Block_2& b) = default;

	int x, y;
	int width;
	int height;
	int color;
};

struct Block_3 { //2x2 블록
	Block_3() = default;
	Block_3(int x, int y, int color)
		: x{ x }, y{ y }, color{ color } {
		width = 2;
		height = 2;
	}
	~Block_3() = default;
	Block_3(const Block_3& b) = default;
	Block_3& operator=(const Block_3& b) = default;

	int x, y;
	int width;
	int height;
	int color;
};