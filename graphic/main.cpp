#include "stdafx.h"
#include "iostream"
#include "windows.h"
#include "d2d1.h"
#include "math.h"
#include "TCHAR.h"
#include "Canvas.h"
#include "matrix.h"
#include "rotation.h"
#include "scene.h"
#include <time.h>  
#include "Dwrite.h"

using namespace std;

HRESULT D2D1Init(HWND hwnd);
LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MyRenderTask();
void Fps();
void MyInput();

ID2D1HwndRenderTarget *pRT = NULL;
IDWriteTextFormat *pTextFormat = NULL;
ID2D1SolidColorBrush *pBrush = NULL;
Canvas *pCanvas = nullptr;
Scene scene;

#define WinSize	1000
RECT fpsRect;
static double deltaTime;

LARGE_INTEGER lastTime, interval;
LARGE_INTEGER Frequency;

int WinMain(HINSTANCE hins, HINSTANCE, PSTR cmd, int cmdShow) {
	const TCHAR CLASS_NAME[] = __T("sample class");
	WNDCLASS wc = {};
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hins;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		__T("learn"),
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,
		NULL,
		hins,
		NULL
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	D2D1Init(hwnd);

	pCanvas = new Canvas();
	pCanvas->Init(pRT);
	MyRenderTask();

	MSG msg = {};

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&lastTime);
	for (;;) {
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				goto _quit;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			LARGE_INTEGER curTime;
			
			QueryPerformanceCounter(&curTime);
			interval.QuadPart = curTime.QuadPart - lastTime.QuadPart;

			interval.QuadPart *= 1000;
			interval.QuadPart /= Frequency.QuadPart;

			if (interval.QuadPart > 33) {
				deltaTime = interval.QuadPart / 1000.0f;
				HWND ahwnd = GetForegroundWindow();
				if (ahwnd == hwnd) {
					MyInput();
					MyRenderTask();
				}
				
				lastTime = curTime;
				//MyLog(_T("%I64d"), interval.QuadPart);
			}
			Sleep(1);
		}
	}
_quit:
	return 0;
}

HRESULT D2D1Init(HWND hwnd) {
	//d2d initialize
	ID2D1Factory *pD2DFactory = NULL;
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	RECT rec;
	GetClientRect(hwnd, &rec);
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			//D2D1::SizeU(rec.right - rec.left, rec.bottom - rec.top)
			D2D1::SizeU(WinSize, WinSize)
		),
		&pRT
	);

	//d2d1 text
	IDWriteFactory *pDWriteFactory = NULL;
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory->CreateTextFormat(
			L"Khmer UI Bold",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			35.0f,
			L"en-us",
			&pTextFormat
		);
	}
	/*if (SUCCEEDED(hr))
	{
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}*/

	pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&pBrush
	);

	GetClientRect(hwnd, &fpsRect);

	return hr;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static bool keyDown = false;
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WVR_REDRAW:
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#define SHIFTED 0x8000 
static double rotationY = 0, rotationX = 0;

void MyInput() { //要用大写字母
	
	MyVector3 motion;
	//前后
	if (GetKeyState('W') & SHIFTED) {
		pz += -deltaTime * 100;
	}
	else if (GetKeyState('S') & SHIFTED) {
		pz += deltaTime * 100;
	}
	//左右
	if (GetKeyState('A') & SHIFTED) {
		px += -deltaTime * 50;
	}
	else if (GetKeyState('D') & SHIFTED) {
		px += deltaTime * 50;
	}

	//上下
	if (GetKeyState('I') & SHIFTED) {
		py += deltaTime * 50;
	}
	else if (GetKeyState('K') & SHIFTED) {
		py += -deltaTime * 50;
	}

	//上下
	if (GetKeyState(VK_UP) & SHIFTED) {
		rx += deltaTime * 50;
	}
	else if (GetKeyState(VK_DOWN) & SHIFTED) {
		rx += -deltaTime * 50;
	}

	//左右
	if (GetKeyState(VK_RIGHT) & SHIFTED) {
		ry += -deltaTime * 50;
	}
	else if (GetKeyState(VK_LEFT) & SHIFTED) {
		ry += deltaTime * 50;
	}

	/*static bool inRotation = false;
	static POINT lastPos;
	if (GetKeyState(VK_RBUTTON) & SHIFTED) {
		if (inRotation == false) {
			if (GetCursorPos(&lastPos)) {
				inRotation = true;
			}
		}
		else
		{
			POINT curPos;
			if (GetCursorPos(&curPos)) {
				LONG x = curPos.x - lastPos.x;
				LONG y = curPos.y - lastPos.y;
				rotationX += -(double)y / 1366.0f * 180;
				rotationY += -(double)x / 768.0f * 180;
				if (rotationX < -89)
					rotationX = -89; 
				if (rotationX > 89)
					rotationX = 89;
				lastPos = curPos;
			}
		}
	}
	else {
		inRotation = false;
	}*/

	/*if (GetKeyState(VK_LBUTTON) & SHIFTED) {
		MyLog(_T("%.2f, %.2f"), rotationX, rotationY);
	}*/

	/*if(motion.x != 0 || motion.y != 0 || motion.z != 0)
		scene.camera.Move(motion);
	scene.camera.transform.rotation.EulerAngles(rotationX, rotationY, 0);*/
}


void MyRenderTask() {
	pCanvas->Clear();
	scene.Render(pCanvas);
	ID2D1Bitmap* bitMap = pCanvas->GetBitMap();
	D2D1_RECT_F rectD = D2D1::RectF(0, 0, WinSize, WinSize);
	D2D1_SIZE_U size = bitMap->GetPixelSize();
	D2D1_RECT_F rectS = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
	pRT->BeginDraw();
	pRT->Clear();
	pRT->DrawBitmap(bitMap, &rectD, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &rectS);
	Fps();
	pRT->EndDraw();
}

void Fps() {
	
	static int fps = 0;
	static double sencond = 0;
	static int frames = 0;

	++frames;
	sencond += deltaTime;
	if (sencond >= 1)
	{
		fps = frames;
		frames = 0;
		sencond = 0;
	}

	static wchar_t strFps[128];
	wsprintfW(strFps, L"FPS %d", fps);
	D2D1_SIZE_F renderTargetSize = pRT->GetSize();
	
	pRT->DrawTextA(
		strFps,
		lstrlenW(strFps),
		pTextFormat,
		D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
		pBrush
	);
}
