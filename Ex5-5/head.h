#pragma once
#include <windows.h>		//--- ������ ��� ����
#include <tchar.h>
#include <vector>
#include <random>

#pragma comment (lib, "msimg32.lib")

std::random_device rd;  // �õ� ������
std::mt19937 gen(rd()); // Mersenne Twister ����
std::uniform_int_distribution<> r_flag(0, 1); // 