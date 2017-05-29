#include "stdafx.h"
#include "Canvas.h"
#include "math.h"


bool doubleEqual(double a, double b) {
	if (abs(a - b) < 0.0001f)
		return true;

	return false;
}

//ɨ����
void Canvas::DrawScanLine(int y, int left, int right, MyVector2* leftUV, MyVector2* rightUV, double leftZ, double rightZ) {
	if (left >= right)
		return;
	MyVector2 uv;
	MyColor textureColor(1,0,0);
	for (int x = left; x <= right; x++) {
		double r = ((double)right - x) / (right - left);
		double p = r * rightZ / (r * rightZ + (1 - r) * leftZ);
		double z = leftZ * p + rightZ * (1 - p);
		/*MyVector2::Interpolate(leftUV, rightUV, p, 1-p, &uv);
		texture->GetPixel(uv.x, uv.y, &textureColor);*/
		SetPixel(x, y, &textureColor, z);
	}
}

//ƽ��,�������»�
void Canvas::DrawFlatTopTriangle(FlatTriangleArg arg) {
	if (abs(arg.flatLeft.y - arg.flatRight.y) > 0.0001f)
		return;

	arg.flatLeft.x *= this->canvasSize - 1;
	arg.flatLeft.y *= this->canvasSize - 1;
	arg.flatRight.x *= this->canvasSize - 1;
	arg.flatRight.y *= this->canvasSize - 1;
	arg.tine.x *= this->canvasSize - 1;
	arg.tine.y *= this->canvasSize - 1;

	int ystart = floor(arg.flatLeft.y);
	int yend = floor(arg.tine.y);
	if (ystart >= yend) //���ʱ�˻���ֱ��,������
		return;

	double leftX, rightX;

	//�������Ҷ˵��uv
	MyVector2 leftUV;
	MyVector2 rightUV;


	for (int y = ystart; y < yend; y++) {
		double r = (arg.tine.y - y) /(arg.tine.y - arg.flatLeft.y);
		if (r > 1) //���ȴ�������,�����,��ʱ����
			r = 1;
		double p = r * arg.tine.z / (r * arg.tine.z + (1 - r) * arg.flatLeft.z);
		//leftUV = arg.flatLeftUV * p + arg.tineUV * (1 - p);
		MyVector2::Interpolate(&arg.flatLeftUV, &arg.tineUV, p, 1-p, &leftUV);
		double leftZ = arg.flatLeft.z * p + arg.tine.z * (1 - p);
		leftX = r * arg.flatLeft.x + (1-r) * arg.tine.x;
		
		p = r * arg.tine.z / (r * arg.tine.z + (1 - r) * arg.flatRight.z);
		//rightUV = arg.flatRightUV * p + arg.tineUV * (1 - p);
		MyVector2::Interpolate(&arg.flatRightUV, &arg.tineUV, p, 1 - p, &rightUV);
		double rightZ = arg.flatRight.z * p + arg.tine.z * (1 - p);
		rightX = r* arg.flatRight.x + (1-r)*arg.tine.x;
		
		int xLeftInt = floor(leftX);
		int xRightInt = floor(rightX) - 1;

		//��ɨ����
		DrawScanLine(y, xLeftInt, xRightInt, &leftUV, &rightUV, leftZ, rightZ);
		
	}
}

//ƽ��������
void Canvas::DrawFlatBottomTriangle(FlatTriangleArg arg) {
	if (abs(arg.flatLeft.y - arg.flatRight.y) > 0.0001f)
		return;

	arg.flatLeft.x *= this->canvasSize - 1;
	arg.flatLeft.y *= this->canvasSize - 1;
	arg.flatRight.x *= this->canvasSize - 1;
	arg.flatRight.y *= this->canvasSize - 1;
	arg.tine.x *= this->canvasSize - 1;
	arg.tine.y *= this->canvasSize - 1;

	int ystart = floor(arg.tine.y);
	int yend = floor(arg.flatLeft.y);
	if (ystart >= yend) //���ʱ�˻���ֱ��,������
		return;

	double leftX, rightX;

	//�������Ҷ˵��uv
	MyVector2 leftUV;
	MyVector2 rightUV;

	for (int y = ystart; y < yend; y++) {
		double r = (arg.flatLeft.y - y) / (arg.flatLeft.y - arg.tine.y);
		if (r > 1)
			r = 1;
		double p = r * arg.flatLeft.z / (r * arg.flatLeft.z + (1 - r) * arg.tine.z);
		//leftUV = arg.tineUV * p + arg.flatLeftUV * (1 - p);
		MyVector2::Interpolate(&arg.tineUV, &arg.flatLeftUV, p, 1-p, &leftUV);
		double leftZ = arg.tine.z * p + arg.flatLeft.z * (1 - p);
		leftX = arg.tine.x * r + arg.flatLeft.x * (1 - r);

		p = r * arg.flatRight.z / (r * arg.flatRight.z + (1 - r) * arg.tine.z);
		//rightUV = arg.tineUV * p + arg.flatRightUV * (1 - p);
		MyVector2::Interpolate(&arg.tineUV, &arg.flatRightUV, p, 1-p, &rightUV);
		double rightZ = arg.tine.z * p + arg.flatRight.z * (1 - p);
		rightX = arg.tine.x * r + arg.flatRight.x * (1 - r);

		int xLeftInt = floor(leftX);
		int xRightInt = floor(rightX) - 1;

		//��ɨ����
		DrawScanLine(y, xLeftInt, xRightInt, &leftUV, &rightUV, leftZ, rightZ);
	}
}


void Canvas::DrawTriangle(Vertex *v0, Vertex *v1, Vertex *v2) {
	//��ndc��,��y�Ӵ�С���ж���,ת������Ļ����ʱ,y���Ǵ�С����
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
	if (v2Y == v0Y) { //�������y��ͬ,���뻭��
		return;
	}

	double r = (v2->homoCoord.pos.y - v1->homoCoord.pos.y) /
		(v2->homoCoord.pos.y - v0->homoCoord.pos.y);
	double p = r * v2->homoCoord.w / (r*v2->homoCoord.w + (1 - r)*v0->homoCoord.w);
	//�����˵�,����ƽ��/��
	Vertex split;
	split.homoCoord = v2->homoCoord * (1 - r) + v0->homoCoord * r;
	split.homoCoord.w = v2->homoCoord.w * (1 - p) + v0->homoCoord.w * p; //�����Ϣ,����w
	split.uv = v2->uv * (1 - p) + v0->uv * p;
	bool splitLeft = split.homoCoord.pos.x < v1->homoCoord.pos.x;
	FlatTriangleArg arg;
	//��������
	arg.flatLeft = MyVector3(splitLeft ? split.homoCoord.pos.x : v1->homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 
		splitLeft ? split.homoCoord.w : v1->homoCoord.w);
	arg.flatLeft.x = (arg.flatLeft.x + 1) / 2;
	arg.flatRight = MyVector3(splitLeft ? v1->homoCoord.pos.x : split.homoCoord.pos.x, (1 - v1->homoCoord.pos.y) / 2, 
		splitLeft ? v1->homoCoord.w : split.homoCoord.w);
	arg.flatRight.x = (arg.flatRight.x + 1) / 2;
	//����uv
	arg.flatLeftUV = splitLeft ? split.uv : v1->uv;
	arg.flatRightUV = splitLeft ? v1->uv : split.uv;

	if (v0Y != v1Y) { //v0��v1��y��ͬ,���Ի�һ��ƽ��������
		//�������겢���������
		arg.tine = MyVector3((v0->homoCoord.pos.x + 1) / 2, (1 - v0->homoCoord.pos.y) / 2, v0->homoCoord.w);
		//����uv
		arg.tineUV = v0->uv;
		//draw
		DrawFlatBottomTriangle(arg);
	}

	if (v1Y != v2Y) { //v1��v2��y��ͬ,���Ի�һ��ƽ��������
		//�������겢���������
		arg.tine = MyVector3((v2->homoCoord.pos.x + 1) / 2, (1 - v2->homoCoord.pos.y) / 2, v2->homoCoord.w);
		//����uv
		arg.tineUV = v2->uv;
		//draw
		DrawFlatTopTriangle(arg);
	}
}