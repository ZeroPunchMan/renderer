#include "stdafx.h"
#include "iostream"
#include "windows.h"
#include "d2d1.h"
#include "math.h"
#include "TCHAR.h"
#include "RenderTexture.h"
#include "matrix.h"
#include "rotation.h"
#include "scene.h"
#include <time.h>  
#include "Dwrite.h"
#include "common.h"

using namespace std;
using namespace MyMath;

HRESULT D2D1Init(HWND hwnd);
LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MyRenderTask();
void Info();
void MyInput();
void ParseCmd(const char* cmd);

HWND hwnd = NULL;
ID2D1HwndRenderTarget *pRT = NULL;
IDWriteTextFormat *pTextFormat = NULL;
ID2D1SolidColorBrush *pBrush = NULL;
RenderTexture *pRenderTexture = nullptr;
Scene* pScene;

bool bShowMesh = true;
#define RTSize	1000
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

	/*RECT clientRect = {0,0,600,600};
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, false);*/

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		__T("renderer"),
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		/*0,0,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,*/
		NULL,
		NULL,
		hins,
		NULL
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);

	ParseCmd(cmd);

	D2D1Init(hwnd);

	pRenderTexture = new RenderTexture();
	pRenderTexture->Init(pRT);
	

	pScene = new Scene();

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

	
	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			//D2D1::SizeU(rec.right - rec.left, rec.bottom - rec.top)
			D2D1::SizeU(RTSize, RTSize)
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

	pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&pBrush
	);

	GetClientRect(hwnd, &fpsRect);

	return hr;
}

bool bRotating = false;
bool bMoving = false;
POINT lastMousePos;
LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static bool keyDown = false;
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WVR_REDRAW:
		return 0;
	case WM_RBUTTONDOWN:
		if (!bMoving) {
			GetCursorPos(&lastMousePos);
			bRotating = true;
		}
		break;
	case WM_RBUTTONUP:
		bRotating = false;
		break;
	case WM_LBUTTONDOWN:
		if (!bRotating) {
			GetCursorPos(&lastMousePos);
			bMoving = true;
		}
		break;
	case WM_LBUTTONUP:
		bMoving = false;
		break;
	case WM_MOUSEWHEEL:
		float zDelta = (short)HIWORD(wParam);
		float fov = pScene->camera.GetFov();
		fov -= zDelta * 0.02;
		pScene->camera.SetFov(fov);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#define SHIFTED 0x8000 
static double rotationY = 0, rotationX = 0;

void MyInput() { //要用大写字母
	static float yaw = 0;
	static float pitch = 0;
	if (bRotating || bMoving) {
		POINT curMousePos;
		GetCursorPos(&curMousePos);
		long deltaX = curMousePos.x - lastMousePos.x;
		long deltaY = curMousePos.y - lastMousePos.y;

		int  screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int  screenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (bRotating) {
			float deltaYaw = -(float)deltaX / screenWidth * 180;
			float deltaPitch = -(float)deltaY / screenHeight * 180;
			yaw += deltaYaw;
			pitch += deltaPitch;
			pScene->camera.transform.rotation.EulerAngles(pitch, yaw, 0);

			
		}
		else if (bMoving) {
			pScene->camera.transform.position -= 
				pScene->camera.transform.rotation * Vector3(1, 0, 0) * ((float)deltaX / screenWidth * 50);

			pScene->camera.transform.position +=
				pScene->camera.transform.rotation * Vector3(0, 1, 0) * ((float)deltaY / screenHeight * 50);
		}

		lastMousePos = curMousePos;
	}

	//前后
	if (GetKeyState('W') & SHIFTED) {
		pScene->camera.transform.position += pScene->camera.transform.rotation * Vector3(0,0,-1) * (deltaTime * 100);
	}
	else if (GetKeyState('S') & SHIFTED) {
		pScene->camera.transform.position += pScene->camera.transform.rotation * Vector3(0, 0, -1) * (-deltaTime * 100);
	}
	//左右
	if (GetKeyState('A') & SHIFTED) {
		pScene->camera.transform.position += pScene->camera.transform.rotation * Vector3(1, 0, 0) * (-deltaTime * 100);
	}
	else if (GetKeyState('D') & SHIFTED) {
		pScene->camera.transform.position += pScene->camera.transform.rotation * Vector3(1, 0, 0) * (deltaTime * 100);
	}
}


void MyRenderTask() {
	pRenderTexture->Clear();
	pScene->Render(pRenderTexture);
	ID2D1Bitmap* bitMap = pRenderTexture->GetBitMap();
	D2D1_RECT_F rectD = D2D1::RectF(0, 0, RTSize, RTSize);
	D2D1_SIZE_U size = bitMap->GetPixelSize();
	D2D1_RECT_F rectS = D2D1::RectF(0, 0, size.width - 1, size.height - 1);
	pRT->BeginDraw();
	pRT->Clear();
	pRT->DrawBitmap(bitMap, &rectD, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &rectS);
	Info();
	pRT->EndDraw();
}

void Info() {
	
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
	wsprintfW(strFps, L"FPS %d\nFOV %d", fps, (int)pScene->camera.GetFov());
	D2D1_SIZE_F renderTargetSize = pRT->GetSize();
	
	pRT->DrawTextA(
		strFps,
		lstrlenW(strFps),
		pTextFormat,
		D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
		pBrush
	);
}

void ErrorBox(const char* msg) {
	MessageBox(hwnd, msg, "error", MB_OK);
	exit(1);
}

void ParseCmd(const char* cmd) {
	static bool parsed = false;
	if (!parsed) {
		CLog("parse");
		for (int i = 1; i < __argc; i++) {
			const char* pCmd = __argv[i];
			CLog(pCmd);
			if (pCmd[0] != '-') {
				ErrorBox("cmd line error");
			}
			switch (pCmd[1]) {
			case 'f': //fbx
				modelFile = __argv[++i];
				break;
			case 't': //texture
				textureFile = __argv[++i];
				break;
			//case 'm': //mesh
			//	bShowMesh = true;
			//	break;
			default:
				ErrorBox("cmd line error");
				break;
			}
		}

		parsed = true;
	}
}
