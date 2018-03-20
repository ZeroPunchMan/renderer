#include "stdafx.h"
#include "MyLog.h"

void CLog(const TCHAR* format, ...) {
	va_list va;
	va_start(va, format);
	TCHAR log[512];
	vsprintf(log, format, va);
	OutputDebugString(log);
	OutputDebugString("\n");
}
