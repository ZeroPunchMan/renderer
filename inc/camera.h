#pragma once

#include <vector>
#include "transform.h"
#include "math3d.h"
#include "matrix.h"
#include "RenderTexture.h"
#include "vertex.h"
#include "FbxModel.h"

class Camera
{
public:
	Camera() {
		Setup(-1, -1000, 1, 90);
	}

	Camera(float n, float f, float aspect, float fov) {
		Setup(n, f, aspect, fov);
	}

	void Setup(float n, float f, float aspect, float fov)
	{
		this->fov = fov;
		this->aspect = aspect;

		this->n = n;
		this->f = f;
		this->b = n * tan(fov * PI / 360);
		this->t = -this->b;
		this->l = this->b * aspect;
		this->r = this->t * aspect;
		SetupFrumstum();
	}

	~Camera() {}

	Transform transform;

	void SetFov(float newFov) {
		if (newFov > 120)
			newFov = 120;
		else if (newFov < 50)
			newFov = 50;
		Setup(n, f, aspect, newFov);
	}
	
	float GetFov() {
		return this->fov;
	}

	void LookAt(MyMath::Vector3 point) {
		//没有做
	}

	MyMath::HomoVector4 PerspectiveProj(MyMath::HomoVector4 point) {
		MyMath::HomoVector4 newPoint = this->perspect * point;
		return newPoint;
	}

	//z为向前,y为向上,x为向左
	void Move(MyMath::Vector3 dist) {
		MyMath::Vector3 mov(-dist.x, dist.y, -dist.z);
		mov = transform.rotation * mov;
		transform.position += mov;
	}

	//渲染模型
	void RenderModel(RenderTexture* pRenderTexture, FbxModel *pModel);

	//画三角形
	void DrawTriangle(RenderTexture* pRenderTexture, Vertex v0, Vertex v1, Vertex v2);

	MyMath::MyMat4 perspect;
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
	
	MyMath::HomoVector4 WorldToCamera(MyMath::HomoVector4 point);
	bool FrustumCull(Vertex *v0, Vertex *v1, Vertex *v2);
	bool BackFaceCull(Vertex *v0, Vertex *v1, Vertex *v2);
	void HomoClip(vector<Vertex> *in, vector<Vertex> *out);
	void Triangulate(vector<Vertex> *in, vector<int> *out);
	
	
	typedef bool(*VertexInternalFunc)(Vertex *pV, Camera *pCamera);  //判断是否在内部
	typedef void(*IntersectionFunc)(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera); //计算交点
	
	//齐次裁减,传入不同的函数,裁减不同的平面
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

	double l, r, t, b, n, f; //frustum参数
	double leftBd, rightBd, topBd, bottomBd;
	float fov, aspect;
};


