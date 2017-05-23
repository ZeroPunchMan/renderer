#include "stdafx.h"

//void Canvas::LineBres(float xs, float ys, float xe, float ye) {
//	int xSrc = xs * (canvasSize - 1);
//	int ySrc = ys * (canvasSize - 1);
//	int xEnd = xe * (canvasSize - 1);
//	int yEnd = ye * (canvasSize - 1);
//	int dx = xEnd - xSrc;
//	int dy = yEnd - ySrc;
//
//	int exg = abs(dy) <= abs(dx) ? 0 : 1;
//	int minusX = dx < 0;
//	int minusY = dy < 0;
//
//	int vdx = abs(dx), vdy = abs(dy);
//	if (exg) {
//		vdx = abs(dy);
//		vdy = abs(dx);
//	}
//
//	int p = 2 * vdy - vdx;
//	SetPixel(xSrc, ySrc, MyColor(1.0f, 0, 0, 1.0f));
//	int y = 0;
//	for (int x = 1; x <= vdx; x++){
//		if (p >= 0){
//			y++;
//		}
//		else {
//			y = y;
//		}
//
//		int addX = x, addY = y;
//		if (exg){
//			addX = y;
//			addY = x;
//		}
//		if (minusX)
//			addX = -addX;
//		if (minusY)
//			addY = -addY;
//		SetPixel(xSrc + addX, ySrc + addY, MyColor(1.0f, 0, 0, 1.0f));
//		p = p + 2 * (vdy + vdx * (p >= 0 ? -1 : 0));
//	}
//}
//
//void Canvas::LineDDA(float xs, float ys, float xe, float ye){
//	int xSrc = xs * (canvasSize - 1);
//	int ySrc = ys * (canvasSize - 1);
//	int xEnd = xe * (canvasSize - 1);
//	int yEnd = ye * (canvasSize - 1);
//	int dx = xEnd - xSrc;
//	int dy = yEnd - ySrc;
//
//	float incX, incY;
//	float steps;
//
//	if (abs(dx) < abs(dy)){ //k < 1
//		steps = (float)abs(dy);
//	}
//	else { //k >= 1
//		steps = (float)abs(dx);
//	}
//	incX = (float)dx / steps;
//	incY = (float)dy / steps;
//
//	float x = xSrc, y = ySrc;
//	for (int i = 0; i <= steps; i++){
//		SetPixel(x, y, 0xff0000);
//		x += incX;
//		y += incY;
//	}
//}