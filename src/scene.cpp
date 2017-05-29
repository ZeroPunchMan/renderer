#include "stdafx.h"
#include "scene.h"
#include "FbxModel.h"

float rx = 0, ry = 0, rz = 0;
float px = 0, py = 0, pz = -50;

FbxModel *model;
Scene::Scene()
{
	model = FbxModel::ImportFbxModel("../../resources/teapot.FBX");
}

Scene::~Scene()
{

}

void Scene::Render(Canvas* pCanvas) {
	model->transform.position = MyVector3(px, py, pz);
	model->transform.rotation.EulerAngles(rx, ry, rz);
	this->camera.RenderModel(pCanvas, model);
}