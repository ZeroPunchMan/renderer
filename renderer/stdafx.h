#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "MyLog.h"
#include "stdlib.h"

using namespace std;
template <typename T>
void Swap(T &a, T &b) {
	T temp = a;
	a = b;
	b = temp;
}
