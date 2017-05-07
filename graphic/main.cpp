#include "stdafx.h"
#include "iostream"
#include "windows.h"
#include "d2d1.h"
#include "math.h"
#include "TCHAR.h"
#include "Canvas.h"
#include "triangle.h"

using namespace std;

HRESULT D2D1Init(HWND hwnd);
LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MyRenderTask();

ID2D1HwndRenderTarget *pRT = NULL;
Canvas *pCanvas = nullptr;

#define WinSize	3000

int WinMain(HINSTANCE hins, HINSTANCE, PSTR cmd, int cmdShow){
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

	if (hwnd == NULL){
		return 0;
	}

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	D2D1Init(hwnd);

	pCanvas = new Canvas();
	pCanvas->Init(pRT);
	MyRenderTask();

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

HRESULT D2D1Init(HWND hwnd){
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

	return hr;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			//MyRenderTask();
			return 0;
		case WVR_REDRAW:
			MyRenderTask();
			return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MyRenderTask(){
	Vertex v0,v1,v2;
	v0.color = MyColor(1.0f, 0, 0, 1);
	v1.color = MyColor(0, 1.0f, 0, 1);
	v2.color = MyColor(0, 0, 1.0f, 1);

	v0.homoCoord = MyVector4(0.5f, 0, 0, 0);
	v1.homoCoord = MyVector4(0.51f, 0.5f, 0, 0);
	v2.homoCoord = MyVector4(0.49f, 1, 0, 0); 

	pCanvas->Clear();
	pCanvas->DrawTriangle(&v0, &v1, &v2);

	MyLog(_T("%.2f, %d, xxxxx"), 10.3f, 20);

	ID2D1Bitmap* bitMap = pCanvas->GetBitMap();
	D2D1_RECT_F rectD = D2D1::RectF(0, 0, WinSize, WinSize);
	D2D1_SIZE_U size = bitMap->GetPixelSize();
	D2D1_RECT_F rectS = D2D1::RectF(0, 0, size.width-1, size.height-1);
	pRT->BeginDraw();
	pRT->DrawBitmap(bitMap, &rectD, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &rectS);
	pRT->EndDraw();
}

