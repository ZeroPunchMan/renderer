#include "stdafx.h"
#include "Canvas.h"
#include "math.h"

//#define SWAP(x, y) {x^=y; y^=x; x^=y;}

bool floatEqual(float a, float b) {
	if (abs(a - b) < 0.0001f)
		return true;

	return false;
}

//扫描线
void Canvas::DrawLine(int y, int left, int right, MyColor* leftColor, MyColor* stepColor) {
	MyColor color = *leftColor;
	//float z = leftZ;
	for (int i = left; i <= right; i++) {
		SetPixel(i, y, &color);
		//DrawPixel(i, y, z, &color);
		//z += stepZ;
		color += *stepColor;
	}
}

//平顶
void Canvas::DrawFlatTopTriangle(FlatTriangleArg *arg) {
	if (abs(arg->flatLeft.y - arg->flatRight.y) > 0.0001f)
		return;

	int ystart = ceil(arg->flatLeft.y);
	int yend = ceil(arg->tine.y);
	if (ystart >= yend) //相等时退化成直线,不画了
		return;

	float xLeft, xRight;
	xLeft = ceil(arg->flatLeft.x);
	xRight = ceil(arg->flatRight.x);
	float xLeftStep, xRightStep;
	xLeftStep = (arg->tine.x - arg->flatLeft.x) / (yend - ystart);
	xRightStep = (arg->tine.x - arg->flatRight.x) / (yend - ystart);
	MyColor leftColor = arg->flatLeftColor;
	MyColor rightColor = arg->flatRightColor;
	MyColor leftStepColor = (arg->tineColor - arg->flatLeftColor) / (yend - ystart);
	MyColor rightStepColor = (arg->tineColor - arg->flatRightColor) / (yend - ystart);

	//y边界限制
	if (ystart < 0) {
		int diff = 0 - ystart;
		xLeft += diff * xLeftStep;
		xRight += diff * xRightStep;
		leftColor += leftStepColor * diff;
		rightColor += rightStepColor * diff;
		ystart = 0;
	}
	if (yend >= canvasSize) {
		yend = canvasSize - 1;
	}

	for (int y = ystart; y < yend; y++) {
		int xMin = ceil(xLeft);
		int xMax = ceil(xRight) - 1;
		MyColor stepColor(0, 0, 0, 0);
		if (xMin < xMax)
			stepColor = (rightColor - leftColor) / (xMax - xMin);
		//x边界限制
		MyColor minColor = leftColor;
		if (xMin < 0) {
			int diff = 0 - xMin;
			minColor += stepColor * diff;
			xMin = 0;
		}
		if (xMax >= canvasSize) {
			xMax = canvasSize - 1;
		}
		//画扫描线
		DrawLine(y, xMin, xMax, &minColor, &stepColor);
		//步进坐标和颜色
		xLeft += xLeftStep;
		xRight += xRightStep;
		leftColor += leftStepColor;
		rightColor += rightStepColor;
	}
}

//平底三角形
void Canvas::DrawFlatBottomTriangle(FlatTriangleArg *arg) {
	if (abs(arg->flatLeft.y - arg->flatRight.y) > 0.0001f)
		return;

	int ystart = ceil(arg->tine.y);
	int yend = ceil(arg->flatLeft.y);
	if (ystart >= yend) //相等时退化成直线,不画了
		return;

	float xLeft, xRight;
	xLeft = xRight = ceil(arg->tine.x);
	float xLeftStep, xRightStep;
	xLeftStep = (arg->flatLeft.x - arg->tine.x) / (yend - ystart);
	xRightStep = (arg->flatRight.x - arg->tine.x) / (yend - ystart);
	MyColor leftColor = arg->tineColor;
	MyColor rightColor = arg->tineColor;
	MyColor leftStepColor = (arg->flatLeftColor - arg->tineColor) / (yend - ystart);
	MyColor rightStepColor = (arg->flatRightColor - arg->tineColor) / (yend - ystart);

	//y边界限制
	if (ystart < 0) {
		int diff = 0 - ystart;
		xLeft += diff * xLeftStep;
		xRight += diff * xRightStep;
		leftColor += leftStepColor * diff;
		rightColor += rightStepColor * diff;
		ystart = 0;
	}
	if (yend >= canvasSize) {
		yend = canvasSize - 1;
	}

	for (int y = ystart; y < yend; y++) {
		int xMin = ceil(xLeft);
		int xMax = ceil(xRight) - 1;
		MyColor stepColor(0,0,0,0);
		if (xMin < xMax)
			stepColor = (rightColor - leftColor) / (xMax - xMin);

		//x边界限制
		MyColor minColor = leftColor;
		if (xMin < 0) {
			int diff = 0 - xMin;
			minColor += stepColor * diff;
			xMin = 0;
		}
		if (xMax >= canvasSize) {
			xMax = canvasSize - 1;
		}
		//画扫描线
		DrawLine(y, xMin, xMax, &minColor, &stepColor);
		//步进坐标和颜色
		xLeft += xLeftStep;
		xRight += xRightStep;
		leftColor += leftStepColor;
		rightColor += rightStepColor;
	}
}

void Canvas::DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2) {
	/*MyLog(v0->homoCoord.ToString().c_str());
	MyLog(v1->homoCoord.ToString().c_str());
	MyLog(v2->homoCoord.ToString().c_str());*/
	if (v0->homoCoord.pos.y > v2->homoCoord.pos.y) {

	}
	else
	{
		//temp = v2;
		//v2 = v0;
		//v0 = temp;
		Swap(v0, v2);
	}

	if (v1->homoCoord.pos.y > v0->homoCoord.pos.y) {
		/*temp = v1;
		v1 = v0;
		v0 = temp;*/
		Swap(v0, v1);
	}
	else if (v1->homoCoord.pos.y < v2->homoCoord.pos.y) {
		/*temp = v1;
		v2 = v1;
		v1 = temp;*/
		Swap(v1, v2);
	}
	/*MyLog(_T("after----------------------"));
	MyLog(v0->homoCoord.ToString().c_str());
	MyLog(v1->homoCoord.ToString().c_str());
	MyLog(v2->homoCoord.ToString().c_str());*/

	int v0Y, v1Y, v2Y;
	v0Y = (1 - v0->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	v1Y = (1 - v1->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	v2Y = (1 - v2->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	if (v2Y == v0Y) { //三个点的y相同,不想画了
		//int leftX, rightX, y;
		//float leftZ, rightZ;
		//MyColor leftColor, rightColor;
		//y = v0Y;
		
		//DrawScanningLine(y, leftX, leftZ, &leftColor, rightX, rightZ, &rightColor);

		return;
	}

	float rate = (v1->homoCoord.pos.y - v0->homoCoord.pos.y) /
		(v2->homoCoord.pos.y - v0->homoCoord.pos.y);
	//MyLog(_T("%.2f"), rate);
	Vertex split;
	split.homoCoord = v2->homoCoord * rate + v0->homoCoord * (1 - rate);
	split.color = v2->color * rate + v0->color * (1 - rate);
	bool splitLeft = split.homoCoord.pos.x < v1->homoCoord.pos.x;
	FlatTriangleArg arg;
	arg.flatLeft = MyVector3(splitLeft ? split.homoCoord.pos.x : v1->homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 0);
	arg.flatLeft.x = (arg.flatLeft.x + 1) / 2;
	arg.flatRight = MyVector3(splitLeft ? v1->homoCoord.pos.x : split.homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 0);
	arg.flatRight.x = (arg.flatRight.x + 1) / 2;
	arg.flatLeftColor = splitLeft ? split.color : v1->color;
	arg.flatRightColor = splitLeft ? v1->color : split.color;

	arg.flatLeft *= this->canvasSize - 1;
	arg.flatRight *= this->canvasSize - 1;

	//arg.flatLeftZ = splitLeft ? split.homoCoord.z : v1->homoCoord.z;
	//arg.flatRightZ = splitLeft ? v1->homoCoord.z : split.homoCoord.z;\

	if (v0Y != v1Y) { //v0和v1的y不同,可以画一个平底三角形
		arg.tine = MyVector3((v0->homoCoord.pos.x + 1) / 2, (1 - v0->homoCoord.pos.y) / 2, v0->homoCoord.pos.z);
		//MyLog(arg.tine.ToString().c_str());
		arg.tine *= this->canvasSize - 1;
		arg.tineColor = v0->color;
		DrawFlatBottomTriangle(&arg);
	}

	if (v1Y != v2Y) { //v1和v2的y不同,可以画一个平顶三角形
		arg.tine = MyVector3((v2->homoCoord.pos.x + 1) / 2, (1 - v2->homoCoord.pos.y) / 2, v2->homoCoord.pos.z);
		//MyLog(arg.tine.ToString().c_str());
		arg.tine *= this->canvasSize - 1;
		arg.tineColor = v2->color;
		DrawFlatTopTriangle(&arg);
	}
}