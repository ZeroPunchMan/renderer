#pragma once

#include "windows.h"
#include "d2d1.h"
#include "triangle.h"

//平底/顶三角形参数
struct FlatTriangleArg {
	MyVector2Int tine;
	float tineZ;
	MyColor *tineColor;

	int flatLeftX, flatRightX, flatY;
	float flatLeftZ, flatRightZ;
	MyColor  *flatLeftColor, *flatRightColor;
};


class Canvas
{
	
public:
	
	Canvas();
	virtual ~Canvas(){};

	HRESULT Init(ID2D1HwndRenderTarget *pRT);

	ID2D1Bitmap* GetBitMap();

	//void LineBres(float xs, float ys, float xe, float ye);
	//void LineDDA(float xs, float ys, float xe, float ye);
	
	void DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2);

	//colors顺序为top,leftbottom, rightbottom
	void DrawFlatTriangle(FlatTriangleArg *arg);

	void Clear();

private:
	ID2D1Bitmap *pBitMap = NULL;
	const static int canvasSize = 3000;
	UINT32 bitMap[canvasSize][canvasSize];
	float zBuffer[canvasSize][canvasSize];

	inline void SetPixel(int x, int y, MyColor* c) {
		uint8_t r = c->r * 255;
		uint8_t g = c->g * 255;
		uint8_t b = c->b * 255;
		bitMap[y][x] = (UINT32)r << 16 | (UINT32)g << 8 | b; // x y reverse
	}
	//void SetPixel(int x, int y, UINT32 color); { bitMap[y][x] = color; }
	
	void DrawPixel(int x, int y, float z, MyColor* c);

	void DrawScanningLine(int y, int left, float leftZ, MyColor* leftColor, int right, float rightZ, MyColor* rightColor);
};


