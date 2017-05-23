#pragma once

#include <vector>
#include "canvas.h"
#include "camera.h"
#include "texture.h"

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
	HomoPoint3 CameraToNDC(HomoPoint3 point);

	
};



