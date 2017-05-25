#pragma once

#include "transform.h"
#include "matrix.h"

class Camera
{
public:
	Camera() {
		l = -1.0f;
		r = 1.0f;
		t = 1.0f;
		b = -1.0f;
		n = -0.3f;
		f = -1000.0f;
		SetupFrumstum();
	}
	~Camera() {}

	double l, r, t, b, n, f; //frustum参数

	Transform transform;

	void LookAt(MyVector3 point) {

	}

	HomoPoint3 PerspectiveProj(HomoPoint3 point) {
		HomoPoint3 newPoint = this->perspect * point;
		return newPoint;
	}

	//z为向前,y为向上,x为向左
	void Move(MyVector3 dist) {
		MyVector3 mov(-dist.x, dist.y, -dist.z);
		mov = transform.rotation * mov;
		transform.position += mov;
	}

	MyMat4 perspect;
private:
	
	void SetupFrumstum() {
		perspect.data[0][0] = 2 * n / (r - l);
		perspect.data[0][1] = 0;
		perspect.data[0][2] = -(r + l) / (r - l);
		perspect.data[0][3] = 0;

		perspect.data[1][0] = 0;
		perspect.data[1][1] = 2 * n / (t - b);
		perspect.data[1][2] = -(t + b) / (t - b);
		perspect.data[1][3] = 0;

		perspect.data[2][0] = 0;
		perspect.data[2][1] = 0;
		perspect.data[2][2] = (f + n) / (f - n);
		perspect.data[2][3] = -2 * f * n / (f - n);

		perspect.data[3][0] = 0;
		perspect.data[3][1] = 0;
		perspect.data[3][2] = 1;
		perspect.data[3][3] = 0;
	}
};


