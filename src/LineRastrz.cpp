#include "stdafx.h"
#include "Canvas.h"

void Canvas::LineBres(Vertex *v0, Vertex *v1, MyColor *color) {
	int xSrc = (v0->homoCoord.pos.x + 1) * (canvasSize - 1) / 2;
	int ySrc = (1 - v0->homoCoord.pos.y) * (canvasSize - 1) / 2;
	int xEnd = (v1->homoCoord.pos.x + 1) * (canvasSize - 1) / 2;
	int yEnd = (1 - v1->homoCoord.pos.y) * (canvasSize - 1) / 2;

	int dx = xEnd - xSrc;
	int dy = yEnd - ySrc;

	int exg = abs(dy) <= abs(dx) ? 0 : 1;
	int minusX = dx < 0;
	int minusY = dy < 0;

	int vdx = abs(dx), vdy = abs(dy);
	if (exg) {
		vdx = abs(dy);
		vdy = abs(dx);
	}


	int a = 2 * vdy - vdx;
	SetPixelWithouZBuffer(xSrc, ySrc, color);
	int y = 0;
	for (int x = 1; x <= vdx; x++){
		if (a >= 0){
			y++;
		}
		else {
			y = y;
		}

		int addX = x, addY = y;
		if (exg){
			addX = y;
			addY = x;
		}
		if (minusX)
			addX = -addX;
		if (minusY)
			addY = -addY;

		double r = (double)addY / (v1->homoCoord.pos.y - v0->homoCoord.pos.y);
		double p = r * v1->homoCoord.w / (r*v1->homoCoord.w + (1 - r)*v0->homoCoord.w);
		double z = v1->homoCoord.w * (1 - p) + v0->homoCoord.w * p;
		SetPixelWithouZBuffer(xSrc + addX, ySrc + addY, color);
		a = a + 2 * (vdy + vdx * (a >= 0 ? -1 : 0));
	}
}
//
//void Canvas::LineDDA(double xs, double ys, double xe, double ye){
//	int xSrc = xs * (canvasSize - 1);
//	int ySrc = ys * (canvasSize - 1);
//	int xEnd = xe * (canvasSize - 1);
//	int yEnd = ye * (canvasSize - 1);
//	int dx = xEnd - xSrc;
//	int dy = yEnd - ySrc;
//
//	double incX, incY;
//	double steps;
//
//	if (abs(dx) < abs(dy)){ //k < 1
//		steps = (double)abs(dy);
//	}
//	else { //k >= 1
//		steps = (double)abs(dx);
//	}
//	incX = (double)dx / steps;
//	incY = (double)dy / steps;
//
//	double x = xSrc, y = ySrc;
//	for (int i = 0; i <= steps; i++){
//		SetPixel(x, y, 0xff0000);
//		x += incX;
//		y += incY;
//	}
//}