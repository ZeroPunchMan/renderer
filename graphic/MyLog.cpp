#include "stdafx.h"
#include "MyLog.h"

void MyLog(const TCHAR* format, ...) {
	va_list va;
	va_start(va, format);
	TCHAR log[512];
	vswprintf(log, format, va);
	CLogOutput() << log << _T("\n");
}
