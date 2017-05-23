#include "stdafx.h"
#include "Canvas.h"
#include "math.h"


bool floatEqual(float a, float b) {
	if (abs(a - b) < 0.0001f)
		return true;

	return false;
}

//扫描线
void Canvas::DrawLine(int y, int left, int right, MyColor* leftColor, MyColor* stepColor, MyVector2* leftUV, MyVector2* stepUV) {
	MyColor color = *leftColor;
	MyVector2 uv = *leftUV;
	//float z = leftZ;
	for (int i = left; i <= right; i++) {
		MyColor textureColor = texture->GetPixel(uv.x, uv.y);
		textureColor *= color;
		SetPixel(i, y, &textureColor);
		//DrawPixel(i, y, z, &color);
		//z += stepZ;
		color += *stepColor;
		uv += *stepUV;
	}
}

//平顶,从上向下画
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
	//计算左右端点的颜色,以及y递增时对应的步进颜色
	MyColor leftColor = arg->flatLeftColor;
	MyColor rightColor = arg->flatRightColor;
	MyColor leftStepColor = (arg->tineColor - arg->flatLeftColor) / (yend - ystart);
	MyColor rightStepColor = (arg->tineColor - arg->flatRightColor) / (yend - ystart);
	//计算左右端点的uv,步进时的uv
	MyVector2 leftUV = arg->flatLeftUV;
	MyVector2 rightUV = arg->flatRightUV;
	MyVector2 leftStepUV = (arg->tineUV - arg->flatLeftUV) / (yend - ystart);
	MyVector2 rightStepUV = (arg->tineUV - arg->flatRightUV) / (yend - ystart);

	//y边界限制
	if (ystart < 0) { //起始坐标改变,计算插值
		int diff = 0 - ystart;
		xLeft += diff * xLeftStep;
		xRight += diff * xRightStep;
		leftColor += leftStepColor * diff;
		rightColor += rightStepColor * diff;
		leftUV += leftStepUV * diff;
		rightUV += rightStepUV * diff;
		ystart = 0;
	}
	if (yend >= canvasSize) {
		yend = canvasSize - 1;
	}

	for (int y = ystart; y < yend; y++) {
		int xMin = ceil(xLeft);
		int xMax = ceil(xRight) - 1;
		MyColor stepColor(0, 0, 0, 0); //x递增时步进的颜色和uv
		MyVector2 stepUV(0, 0);
		if (xMin < xMax) {
			stepColor = (rightColor - leftColor) / (xMax - xMin);
			stepUV = (rightUV - leftUV) / (xMax - xMin);
		}
		//x边界限制
		MyColor minColor = leftColor;
		MyVector2 minUV = leftUV;
		if (xMin < 0) {
			int diff = 0 - xMin;
			minColor += stepColor * diff;
			minUV += stepUV * diff;
			xMin = 0;
		}
		if (xMax >= canvasSize) {
			xMax = canvasSize - 1;
		}
		//画扫描线
		DrawLine(y, xMin, xMax, &minColor, &stepColor, &minUV, &stepUV);
		//步进坐标和颜色,uv
		xLeft += xLeftStep;
		xRight += xRightStep;
		leftColor += leftStepColor;
		rightColor += rightStepColor;
		leftUV += leftStepUV;
		rightUV += rightStepUV;
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
	//计算颜色,步进颜色
	MyColor leftColor = arg->tineColor;
	MyColor rightColor = arg->tineColor;
	MyColor leftStepColor = (arg->flatLeftColor - arg->tineColor) / (yend - ystart);
	MyColor rightStepColor = (arg->flatRightColor - arg->tineColor) / (yend - ystart);
	//计算左右端点的uv,y步进时的uv
	MyVector2 leftUV = arg->tineUV;
	MyVector2 rightUV = arg->tineUV;
	MyVector2 leftStepUV = (arg->flatLeftUV - arg->tineUV) / (yend - ystart);
	MyVector2 rightStepUV = (arg->flatRightUV - arg->tineUV) / (yend - ystart);

	//y边界限制
	if (ystart < 0) {
		int diff = 0 - ystart;
		xLeft += diff * xLeftStep;
		xRight += diff * xRightStep;
		leftColor += leftStepColor * diff;
		rightColor += rightStepColor * diff;
		leftUV += leftStepUV * diff;
		rightUV += rightStepUV * diff;
		ystart = 0;
	}
	if (yend >= canvasSize) {
		yend = canvasSize - 1;
	}

	for (int y = ystart; y < yend; y++) {
		int xMin = ceil(xLeft);
		int xMax = ceil(xRight) - 1;
		MyColor stepColor(0,0,0,0);
		MyVector2 stepUV(0, 0);
		if (xMin < xMax) {
			stepColor = (rightColor - leftColor) / (xMax - xMin);
			stepUV = (rightUV - leftUV) / (xMax - xMin);
		}

		//x边界限制
		MyColor minColor = leftColor;
		MyVector2 minUV = leftUV;
		if (xMin < 0) {
			int diff = 0 - xMin;
			minColor += stepColor * diff;
			minUV += leftStepUV * diff;
			xMin = 0;
		}
		if (xMax >= canvasSize) {
			xMax = canvasSize - 1;
		}
		//画扫描线
		DrawLine(y, xMin, xMax, &minColor, &stepColor, &minUV, &stepUV);
		//步进坐标和颜色,uv
		xLeft += xLeftStep;
		xRight += xRightStep;
		leftColor += leftStepColor;
		rightColor += rightStepColor;
		leftUV += leftStepUV;
		rightUV += rightStepUV;
	}
}

void Canvas::DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2) {
	if (v0->homoCoord.pos.y > v2->homoCoord.pos.y) {

	}
	else{
		Swap(v0, v2);
	}

	if (v1->homoCoord.pos.y > v0->homoCoord.pos.y) {
		Swap(v0, v1);
	}
	else if (v1->homoCoord.pos.y < v2->homoCoord.pos.y) {
		Swap(v1, v2);
	}

	int v0Y, v1Y, v2Y;
	v0Y = (1 - v0->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	v1Y = (1 - v1->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	v2Y = (1 - v2->homoCoord.pos.y) * (this->canvasSize - 1) / 2;
	if (v2Y == v0Y) { //三个点的y相同,不想画了
		return;
	}

	float rate = (v1->homoCoord.pos.y - v0->homoCoord.pos.y) /
		(v2->homoCoord.pos.y - v0->homoCoord.pos.y);
	//两个端点,公用平底/顶
	Vertex split;
	split.homoCoord = v2->homoCoord * rate + v0->homoCoord * (1 - rate);
	split.color = v2->color * rate + v0->color * (1 - rate);
	split.uv = v2->uv * rate + v0->uv * (1 - rate);
	bool splitLeft = split.homoCoord.pos.x < v1->homoCoord.pos.x;
	FlatTriangleArg arg;
	//计算坐标
	arg.flatLeft = MyVector3(splitLeft ? split.homoCoord.pos.x : v1->homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 0);
	arg.flatLeft.x = (arg.flatLeft.x + 1) / 2;
	arg.flatRight = MyVector3(splitLeft ? v1->homoCoord.pos.x : split.homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 0);
	arg.flatRight.x = (arg.flatRight.x + 1) / 2;
	//计算颜色
	arg.flatLeftColor = splitLeft ? split.color : v1->color;
	arg.flatRightColor = splitLeft ? v1->color : split.color;
	//计算uv
	arg.flatLeftUV = splitLeft ? split.uv : v1->uv;
	arg.flatRightUV = splitLeft ? v1->uv : split.uv;

	//坐标换算成像素
	arg.flatLeft *= this->canvasSize - 1;
	arg.flatRight *= this->canvasSize - 1;

	if (v0Y != v1Y) { //v0和v1的y不同,可以画一个平底三角形
		//计算坐标并换算成像素
		arg.tine = MyVector3((v0->homoCoord.pos.x + 1) / 2, (1 - v0->homoCoord.pos.y) / 2, v0->homoCoord.pos.z);
		arg.tine *= this->canvasSize - 1;  
		//计算颜色
		arg.tineColor = v0->color;
		//计算uv
		arg.tineUV = v0->uv;
		//draw
		DrawFlatBottomTriangle(&arg);
	}

	if (v1Y != v2Y) { //v1和v2的y不同,可以画一个平顶三角形
		//计算坐标并换算成像素
		arg.tine = MyVector3((v2->homoCoord.pos.x + 1) / 2, (1 - v2->homoCoord.pos.y) / 2, v2->homoCoord.pos.z);
		arg.tine *= this->canvasSize - 1;
		//计算颜色
		arg.tineColor = v2->color;
		//计算uv
		arg.tineUV = v2->uv;
		//draw
		DrawFlatTopTriangle(&arg);
	}
}