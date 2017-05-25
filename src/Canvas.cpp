#include "stdafx.h"
#include "Canvas.h"

Texture* texture = NULL;
Canvas::Canvas() {
	texture = new Texture("../resources/awesomeface.png");
}

HRESULT Canvas::Init(ID2D1HwndRenderTarget *pRT) {
	D2D1_PIXEL_FORMAT PixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	HRESULT hr = pRT->CreateBitmap(D2D1::SizeU(canvasSize, canvasSize),
		D2D1::BitmapProperties(PixelFormat),
		&pBitMap
	);
	//if (!SUCCEEDED(hr))
	return hr;
}

ID2D1Bitmap* Canvas::GetBitMap(){
	D2D1_RECT_U rectU = D2D1::RectU(0, 0, canvasSize-1, canvasSize-1);
	HRESULT hr = pBitMap->CopyFromMemory(&rectU,
		bitMap,
		canvasSize * 4
		);
	if (!SUCCEEDED(hr))
		return NULL;

	return this->pBitMap;
}

//void Canvas::DrawPixel(int x, int y, double z, MyColor* c) {
//	if (z > zBuffer[x][y]) {
//		this->SetPixel(x, y, c);
//		zBuffer[x][y] = z;
//	}
//}

void Canvas::Clear() {
	for (int i = 0; i < canvasSize; i++) {
		for (int j = 0; j < canvasSize; j++) {
			zBuffer[i][j] = -100000.0f;
			bitMap[i][j] = 0;
		}
	}
}
