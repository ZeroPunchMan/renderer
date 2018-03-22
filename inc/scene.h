#pragma once

#include <vector>
#include "RenderTexture.h"
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

	
	void Render(RenderTexture*);
};



