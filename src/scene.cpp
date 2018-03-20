#include "stdafx.h"
#include "scene.h"
#include "FbxModel.h"

using namespace MyMath;

float rx = 0, ry = 0, rz = 0;
float px = 0, py = 0, pz = -50;

FbxModel *model;
Scene::Scene() : camera(-1, -1000, 1, 90)
{
	model = FbxModel::ImportFbxModel("../../resources/teapot.FBX");
}

Scene::~Scene()
{

}

void Scene::Render(RenderTexture* pRenderTexture) {
	model->transform.position = Vector3(px, py, pz);
	model->transform.rotation.EulerAngles(rx, ry, rz);
	this->camera.RenderModel(pRenderTexture, model);
}