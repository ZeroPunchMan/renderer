#pragma once

#include <vector>
#include "canvas.h"
#include "camera.h"
#include "texture.h"
#include <list>

extern float rx, ry, rz;
extern float px, py, pz;

using namespace std;
class Scene
{

public:
	Scene();
	~Scene();
	
	Camera camera;

	
	void Render(Canvas*);

	
};



