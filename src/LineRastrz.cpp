#include "stdafx.h"
#include "RenderTexture.h"


using namespace MyMath;

void RenderTexture::LineBres(Vertex *v0, Vertex *v1, const Color& color) {
	int xSrc = (v0->homoCoord.pos.x + 1) * (RTSize - 1) / 2;
	int ySrc = (1 - v0->homoCoord.pos.y) * (RTSize - 1) / 2;
	int xEnd = (v1->homoCoord.pos.x + 1) * (RTSize - 1) / 2;
	int yEnd = (1 - v1->homoCoord.pos.y) * (RTSize - 1) / 2;
	
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


	int p = 2 * vdy - vdx;
	SetPixel(xSrc, ySrc, color, v0->homoCoord.w);
	int y = 0;
	for (int x = 1; x <= vdx; x++){
		if (p >= 0){
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

		double r = (vdx - x) /
			(vdx - 0);
		r = r * v1->homoCoord.w / (r * v1->homoCoord.w + (1 - r) * v0->homoCoord.w);
		float z = v1->homoCoord.w * (1 - r) + v0->homoCoord.w * r + MIRCO_FLOAT;

		SetPixel(xSrc + addX, ySrc + addY, color, z);

		p = p + 2 * (vdy + vdx * (p >= 0 ? -1 : 0));
	}
}
//
//void RenderTexture::LineDDA(double xs, double ys, double xe, double ye){
//	int xSrc = xs * (RTSize - 1);
//	int ySrc = ys * (RTSize - 1);
//	int xEnd = xe * (RTSize - 1);
//	int yEnd = ye * (RTSize - 1);
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