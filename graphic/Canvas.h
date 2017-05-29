#pragma once

#include "windows.h"
#include "d2d1.h"
#include "math3d.h"
#include "vertex.h"
#include "texture.h"

extern Texture* texture;
//ƽ��/�������β���
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

	//x,y����ΪͶӰ���������,wΪʵ�����,xy����Ϊopengl��Ļ����,��Ļ����Ϊ(0,0),���Ͻ�Ϊ(1,1)
	void DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2);

	//x,y����ΪͶӰ���������,wΪʵ�����,xy����Ϊopengl��Ļ����,��Ļ����Ϊ(0,0),���Ͻ�Ϊ(1,1)
	void LineBres(Vertex *v0, Vertex *v1, MyColor *color);

	//ƽ��
	void DrawFlatBottomTriangle(FlatTriangleArg arg);

	//ƽ��
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


