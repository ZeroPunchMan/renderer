#pragma once

#include "windows.h"
#include "d2d1.h"
#include "math3d.h"
#include "vertex.h"
#include "texture.h"

extern Texture* texture;
//平底/顶三角形参数
struct FlatTriangleArg {
	MyVector3 tine;
	MyVector2 tineUV;

	MyVector3 flatLeft;
	MyVector3 flatRight;
	MyVector2 flatLeftUV, flatRightUV;
};


class Canvas
{
	
public:
	
	Canvas();
	virtual ~Canvas(){};

	HRESULT Init(ID2D1HwndRenderTarget *pRT);

	ID2D1Bitmap* GetBitMap();

	//x,y坐标为投影面比例坐标,w为实际深度,xy坐标为opengl屏幕坐标,屏幕中心为(0,0),右上角为(1,1)
	void DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2);

	//x,y坐标为投影面比例坐标,w为实际深度,xy坐标为opengl屏幕坐标,屏幕中心为(0,0),右上角为(1,1)
	void LineBres(Vertex *v0, Vertex *v1, MyColor *color);

	//平底
	void DrawFlatBottomTriangle(FlatTriangleArg arg);

	//平顶
	void DrawFlatTopTriangle(FlatTriangleArg arg);

	void Clear();

private:
	ID2D1Bitmap *pBitMap = NULL;
	const static int canvasSize = 1000;
	UINT32 bitMap[canvasSize][canvasSize];
	double zBuffer[canvasSize][canvasSize];

	inline void SetPixel(int x, int y, MyColor* c, double z) {
		if (x < 0 || x >= canvasSize || y < 0 || y >= canvasSize)
			return;
		if (z <= zBuffer[y][x])
			return;
		zBuffer[y][x] = z;
		uint8_t r = c->r * 255;
		uint8_t g = c->g * 255;
		uint8_t b = c->b * 255;
		bitMap[y][x] = (UINT32)r << 16 | (UINT32)g << 8 | b; // x y reverse
	}

	inline void SetPixelWithouZBuffer(int x, int y, MyColor* c) {
		if (x < 0 || x >= canvasSize || y < 0 || y >= canvasSize)
			return;

		uint8_t r = c->r * 255;
		uint8_t g = c->g * 255;
		uint8_t b = c->b * 255;
		bitMap[y][x] = (UINT32)r << 16 | (UINT32)g << 8 | b; // x y reverse
	}

	void DrawScanLine(int y, int left, int right, MyVector2* leftUV, MyVector2* rightUV, double leftZ, double rigthZ);
};


