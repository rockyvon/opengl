#pragma once

#include "stdafx.h"

template<class T>
int getLength(T* array, int count) {
	return sizeof(array) / count;
}

class Utils{
public:
	static int min(int a, int b);
};