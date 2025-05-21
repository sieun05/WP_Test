#pragma once

struct Tail {
	int type; 
	int tile;

	Tail() = default;
	Tail(int type) : type{ type } {};
};