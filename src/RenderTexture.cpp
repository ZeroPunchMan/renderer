#include "stdafx.h"
#include "RenderTexture.h"

const char* textureFile = "checker.png";
Texture* texture = NULL;
RenderTexture::RenderTexture() {
	texture = new Texture(textureFile);
}

HRESULT RenderTexture::Init(ID2D1HwndRenderTarget *pRT) {
	D2D1_PIXEL_FORMAT PixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	HRESULT hr = pRT->CreateBitmap(D2D1::SizeU(RTSize, RTSize),
		D2D1::BitmapProperties(PixelFormat),
		&pBitMap
	);
	//if (!SUCCEEDED(hr))
	return hr;
}

ID2D1Bitmap* RenderTexture::GetBitMap(){
	D2D1_RECT_U rectU = D2D1::RectU(0, 0, RTSize-1, RTSize-1);
	HRESULT hr = pBitMap->CopyFromMemory(&rectU,
		bitMap,
		RTSize * 4
		);
	if (!SUCCEEDED(hr))
		return NULL;

	return this->pBitMap;
}

void RenderTexture::Clear() {
	for (int i = 0; i < RTSize; i++) {
		for (int j = 0; j < RTSize; j++) {
			zBuffer[i][j] = -100000.0f;
			bitMap[i][j] = 0;
		}
	}
}
