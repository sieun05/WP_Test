#pragma once

struct Block { //¼¼·Î4Ä­ºí·°
	Block() = default;
	Block(int x, int y, int width, int height, int color) 
		: x{ x }, y{ y }, width{ width }, height{ height }, color { color } {

	}
	~Block() = default;
	Block(const Block& b) = default;
	Block& operator=(const Block& b) = default;
	
	int x, y;
	int width;
	int height;
	int color;
};