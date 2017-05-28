#pragma once

#include <vector>
#include "transform.h"
#include "math3d.h"
#include "matrix.h"
#include "Canvas.h"
#include "vertex.h"


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
	double leftBd, rightBd, topBd, bottomBd;

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

	//渲染模型
	void RenderModel(Canvas* pCanvas, Model *pModel);
	void DrawTriangle(Canvas* pCanvas, Vertex v0, Vertex v1, Vertex v2);

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

		leftBd = l / n;
		rightBd = r / n;
		bottomBd = b / n;
		topBd = t / n;
	}

	
	HomoPoint3 WorldToCamera(HomoPoint3 point);
	bool FrustumCull(Vertex *v0, Vertex *v1, Vertex *v2);
	bool BackFaceCull(Vertex *v0, Vertex *v1, Vertex *v2);
	void HomoClip(vector<Vertex> *in, vector<Vertex> *out);
	void Triangulate(vector<Vertex> *in, vector<int> *out);
	
	//齐次裁剪函数
	typedef bool(*VertexInternalFunc)(Vertex *pV, Camera *pCamera);
	typedef void(*IntersectionFunc)(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera);
	void Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection);

	//近平面裁剪
	static bool VertexInNear(Vertex *pV, Camera *pCamera);
	static void IntersectionOnNear(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//远平面裁剪
	static bool VertexInFar(Vertex *pV, Camera *pCamera);
	static void IntersectionOnFar(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//左右算法可能反了,但不影响结果
	//左平面裁剪
	static bool VertexInLeft(Vertex *pV, Camera *pCamera);
	static void IntersectionOnLeft(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//右平面裁剪
	static bool VertexInRight(Vertex *pV, Camera *pCamera);
	static void IntersectionOnRight(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//上平面裁剪
	static bool VertexInTop(Vertex *pV, Camera *pCamera);
	static void IntersectionOnTop(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//下平面裁剪
	static bool VertexInBottom(Vertex *pV, Camera *pCamera);
	static void IntersectionOnBottom(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);
};


