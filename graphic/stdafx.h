#pragma once
#include "MyLog.h"

template <typename T>
void Swap(T &a, T &b) {
	T temp = a;
	a = b;
	b = temp;
}
