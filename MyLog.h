#ifndef _MYLOG_H_
#define _MYLOG_H_

#include "TCHAR.h"
#include "iostream"
#include "windows.h"


#define MyLog(x) (CLogOutput() << x << _T("\n"))


using namespace std;
#ifdef _UNICODE
typedef wstring MyString;
#else
typedef std::string MyString;
#endif

class CLogOutput{
public:
	CLogOutput(){
		
	}
	~CLogOutput(){}

	CLogOutput operator << (float x){
		TCHAR temp[200];
		_stprintf_s(temp, 200, _T("%.2f"), x);
		OutputDebugString(temp);
		return *this;
	}

	CLogOutput operator << (int x){
		TCHAR temp[200];
		_stprintf_s(temp, 200, _T("%d"), x);
		OutputDebugString(temp);
		return *this;
	}

	CLogOutput operator << (TCHAR* x){
		OutputDebugString(x);
		return *this;
	}

	operator bool(){
		return true;
	}
};


#endif
