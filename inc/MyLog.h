#pragma once

#include "TCHAR.h"
#include "iostream"
#include "windows.h"

class CppLog{
public:
	CppLog(){
		
	}
	~CppLog(){}

	CppLog operator << (float x){
		TCHAR temp[200];
		_stprintf_s(temp, 200, _T("%.2f"), x);
		OutputDebugString(temp);
		return *this;
	}

	CppLog operator << (int x){
		TCHAR temp[200];
		_stprintf_s(temp, 200, _T("%d"), x);
		OutputDebugString(temp);
		return *this;
	}

	CppLog operator << (TCHAR* x){
		OutputDebugString(x);
		return *this;
	}

	operator bool(){
		return true;
	}
};

void CLog(const TCHAR* format, ...);

