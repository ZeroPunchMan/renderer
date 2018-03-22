#pragma once

#include "windows.h"
#include "d2d1.h"
#include "math3d.h"
#include "vertex.h"
#include "texture.h"


extern Texture* texture;
//平底/顶三角形参数
struct FlatTriangleArg {
	MyMath::Vector3 tine;
	MyMath::Vector2 tineUV;

	MyMath::Vector3 flatLeft;
	MyMath::Vector3 flatRight;
	MyMath::Vector2 flatLeftUV, flatRightUV;
};


class RenderTexture
{
	
public:
	
	RenderTexture();
	virtual ~RenderTexture(){};

	HRESULT Init(ID2D1HwndRenderTarget *pRT);

	ID2D1Bitmap* GetBitMap();

	//x,y坐标为投影面比例坐标,w为实际深度,xy坐标为opengl屏幕坐标,屏幕中心为(0,0),右上角为(1,1)
	void DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2);

	//x,y坐标为投影面比例坐标,w为实际深度,xy坐标为opengl屏幕坐标,屏幕中心为(0,0),右上角为(1,1)
	void LineBres(Vertex *v0, Vertex *v1, const MyMath::Color& color);

	//平底
	void DrawFlatBottomTriangle(FlatTriangleArg arg);

	//平顶
	void DrawFlatTopTriangle(FlatTriangleArg arg);

	void Clear();

private:
	ID2D1Bitmap *pBitMap = NULL;
	const static int RTSize = 1000;
	UINT32 bitMap[RTSize][RTSize];
	double zBuffer[RTSize][RTSize];

	inline void SetPixel(int x, int y, const MyMath::Color& c, double z) {
		if (x < 0 || x >= RTSize || y < 0 || y >= RTSize)
			return;
		if (z <= zBuffer[y][x])
			return;
		zBuffer[y][x] = z;
		uint8_t r = c.r * 255;
		uint8_t g = c.g * 255;
		uint8_t b = c.b * 255;
		bitMap[y][x] = (UINT32)r << 16 | (UINT32)g << 8 | b; // x y reverse
	}

	inline void SetPixelWithouZBuffer(int x, int y, const MyMath::Color& c) {
		if (x < 0 || x >= RTSize || y < 0 || y >= RTSize)
			return;

		uint8_t r = c.r * 255;
		uint8_t g = c.g * 255;
		uint8_t b = c.b * 255;
		bitMap[y][x] = (UINT32)r << 16 | (UINT32)g << 8 | b; // x y reverse
	}

	void DrawScanLine(int y, int left, int right, MyMath::Vector2* leftUV, MyMath::Vector2* rightUV, double leftZ, double rigthZ);
};


