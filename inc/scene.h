#pragma once

#include <vector>
#include "canvas.h"
#include "camera.h"
#include "texture.h"
#include <list>

using namespace std;
class Scene
{

public:
	Scene();
	~Scene();
	
	Camera camera;

	
	void Render(Canvas*);

	void DrawTriangle(Canvas* pCanvas, Vertex v0, Vertex v1, Vertex v2);
	HomoPoint3 WorldToCamera(HomoPoint3 point);
	bool FrustumCull(Vertex *v0, Vertex *v1, Vertex *v2);
	bool BackFaceCull(Vertex *v0, Vertex *v1, Vertex *v2);
	void HomoClip(vector<Vertex> *in, vector<Vertex> *out);
	void Triangulate(vector<Vertex> *in, vector<int> *out);


	typedef bool (*VertexInternalFunc)(Vertex *pV, Camera *pCamera);
	typedef void (*IntersectionFunc)(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera);
	void Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection);

	//��ƽ��ü�
	static bool VertexInNear(Vertex *pV, Camera *pCamera);
	static void IntersectionOnNear(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//Զƽ��ü�
	static bool VertexInFar(Vertex *pV, Camera *pCamera);
	static void IntersectionOnFar(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//�����㷨���ܷ���,����Ӱ����
	//��ƽ��ü�
	static bool VertexInLeft(Vertex *pV, Camera *pCamera);
	static void IntersectionOnLeft(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//��ƽ��ü�
	static bool VertexInRight(Vertex *pV, Camera *pCamera);
	static void IntersectionOnRight(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//��ƽ��ü�
	static bool VertexInTop(Vertex *pV, Camera *pCamera);
	static void IntersectionOnTop(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);

	//��ƽ��ü�
	static bool VertexInBottom(Vertex *pV, Camera *pCamera);
	static void IntersectionOnBottom(Vertex *pV1, Vertex *pV2, Vertex *pOut, Camera *pCamera);
};



