#include "stdafx.h"
#include "Canvas.h"
#include "math.h"

#define SWAP(x, y) {x^=y; y^=x; x^=y;}

bool floatEqual(float a, float b) {
	if (abs(a - b) < 0.0001f)
		return true;

	return false;
}

//扫描线
void Canvas::DrawScanningLine(int y, int left, float leftZ, MyColor* leftColor, int right, float rightZ,  MyColor* rightColor) {
	for (int i = left; i <= right; i++) {
		float rate = (float)(i - left) / (right - left);
		float z = (1 - rate)*leftZ + rate *rightZ;
		MyColor color = MyColor::Interpolate(leftColor, rightColor, rate);
		DrawPixel(i, y, z, &color);
	}
}

void Canvas::DrawFlatTriangle(FlatTriangleArg *arg) {
	if (arg->flatY == arg->tine.y || arg->flatLeftX > arg->flatRightX)
		return;

	int dir = arg->flatY > arg->tine.y ? 1 : -1;
	int leftX, rightX, y;
	float leftZ, rightZ;
	for (y = arg->tine.y; y != arg->flatY; y+=dir) {
		float d1 = y - arg->flatY;
		float d2 = arg->tine.y - arg->flatY;
		float rate = d1 / d2;
		
		leftX = rate * arg->tine.x + (1 - rate) * arg->flatLeftX;
		rightX = rate * arg->tine.x + (1 - rate) * arg->flatRightX;
		leftZ = rate * arg->tineZ + (1 - rate) * arg->flatLeftZ;
		rightZ = rate * arg->tineZ + (1 - rate) * arg->flatRightZ;
		MyColor c1 = MyColor::Interpolate(arg->flatLeftColor, arg->tineColor, rate);
		MyColor c2 = MyColor::Interpolate(arg->flatRightColor, arg->tineColor, rate);
		DrawScanningLine(y, leftX, leftZ, &c1, rightX, rightZ, &c2);
	}
	DrawScanningLine(y, arg->flatLeftX, arg->flatLeftZ, arg->flatLeftColor, arg->flatRightX, arg->flatRightZ, arg->flatRightColor);
}

void Canvas::DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2) {
	Vertex *temp;
	if (v0->homoCoord.y < v2->homoCoord.y) {

	}
	else
	{
		temp = v0;
		v2 = v0;
		v0 = temp;
	}

	if (v1->homoCoord.y < v0->homoCoord.y) {
		temp = v0;
		v1 = v0;
		v0 = temp;
	}
	else if (v1->homoCoord.y > v2->homoCoord.y) {
		temp = v1;
		v2 = v1;
		v1 = temp;
	}

	int v0Y, v1Y, v2Y;
	v0Y = v0->homoCoord.y * (this->canvasSize - 1);
	v1Y = v1->homoCoord.y * (this->canvasSize - 1);
	v2Y = v2->homoCoord.y * (this->canvasSize - 1);
	if (v2Y == v0Y) { //三个点的y相同,不想画了
		//int leftX, rightX, y;
		//float leftZ, rightZ;
		//MyColor leftColor, rightColor;
		//y = v0Y;
		
		//DrawScanningLine(y, leftX, leftZ, &leftColor, rightX, rightZ, &rightColor);

		return;
	}

	float rate = (v1->homoCoord.y - v0->homoCoord.y) /
		(v2->homoCoord.y - v0->homoCoord.y);

	Vertex split;
	split.homoCoord = v2->homoCoord * rate + v0->homoCoord * (1 - rate);
	split.color = v2->color * rate + v0->color * (1 - rate);
	bool splitLeft = split.homoCoord.x < v1->homoCoord.x;
	FlatTriangleArg arg;
	arg.flatLeftX = splitLeft ? split.homoCoord.x * (canvasSize - 1) : v1->homoCoord.x * (canvasSize - 1);
	arg.flatRightX = splitLeft ? v1->homoCoord.x * (canvasSize - 1) : split.homoCoord.x * (canvasSize - 1);
	arg.flatY = v1->homoCoord.y * (canvasSize - 1);
	arg.flatLeftZ = splitLeft ? split.homoCoord.z : v1->homoCoord.z;
	arg.flatRightZ = splitLeft ? v1->homoCoord.z : split.homoCoord.z;
	arg.flatLeftColor = splitLeft ? &split.color : &v1->color;
	arg.flatRightColor = splitLeft ? &v1->color : &split.color;

	if (v0Y != v1Y) { //v0和v1的y不同,可以画一个平底三角形
		arg.tine = MyVector2Int(v0->homoCoord.x * (canvasSize - 1), v0->homoCoord.y * (canvasSize -1));
		arg.tineZ = v0->homoCoord.z;
		arg.tineColor = &v0->color;
		DrawFlatTriangle(&arg);
	}

	if (v1Y != v2Y) { //v1好v2的y不同,可以画一个平顶三角形
		arg.tine = MyVector2Int(v2->homoCoord.x * (canvasSize - 1), v2->homoCoord.y * (canvasSize - 1));
		arg.tineZ = v2->homoCoord.z;
		arg.tineColor = &v2->color;
		DrawFlatTriangle(&arg);
	}
}