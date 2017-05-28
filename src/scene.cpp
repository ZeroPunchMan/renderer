#include "stdafx.h"
#include "scene.h"

float rx = 0, ry = 0, rz = 0;
float px = 0, py = 0, pz = -300;
Model *model;
Scene::Scene()
{
	model = new Model();
	model->verCount = 6 * 4;
	model->vertices = new Vertex[model->verCount];
	model->triCount = 6 * 2;
	model->triangles = new int[model->triCount * 3];

	float base = 50;
	//ÕýÃæ
	model->vertices[0].homoCoord = HomoPoint3(-base, base, base);
	model->vertices[1].homoCoord = HomoPoint3(base, base, base);
	model->vertices[2].homoCoord = HomoPoint3(base, -base, base);
	model->vertices[3].homoCoord = HomoPoint3(-base, -base, base);

	model->vertices[0].uv = MyVector2(0, 0);
	model->vertices[1].uv = MyVector2(1, 0);
	model->vertices[2].uv = MyVector2(1, 1);
	model->vertices[3].uv = MyVector2(0, 1);

	model->triangles[0] = 0;
	model->triangles[1] = 3;
	model->triangles[2] = 1;

	model->triangles[3] = 1;
	model->triangles[4] = 3;
	model->triangles[5] = 2;

	//ÓÒÃæ
	model->vertices[4].homoCoord = HomoPoint3(base, base, base);
	model->vertices[5].homoCoord = HomoPoint3(base, base, -base);
	model->vertices[6].homoCoord = HomoPoint3(base, -base, -base);
	model->vertices[7].homoCoord = HomoPoint3(base, -base, base);

	model->vertices[4].uv = MyVector2(0, 0);
	model->vertices[5].uv = MyVector2(1, 0);
	model->vertices[6].uv = MyVector2(1, 1);
	model->vertices[7].uv = MyVector2(0, 1);

	model->triangles[6] = 4;
	model->triangles[7] = 7;
	model->triangles[8] = 5;

	model->triangles[9] = 5;
	model->triangles[10] = 7;
	model->triangles[11] = 6;

	//top
	model->vertices[8].homoCoord = HomoPoint3(-base, base, -base);
	model->vertices[9].homoCoord = HomoPoint3(base, base, -base);
	model->vertices[10].homoCoord = HomoPoint3(-base, base, base);
	model->vertices[11].homoCoord = HomoPoint3(base, base, base);

	model->vertices[8].uv = MyVector2(0, 0);
	model->vertices[9].uv = MyVector2(1, 0);
	model->vertices[10].uv = MyVector2(0, 1);
	model->vertices[11].uv = MyVector2(1, 1);

	model->triangles[12] = 8;
	model->triangles[13] = 10;
	model->triangles[14] = 9;

	model->triangles[15] = 9;
	model->triangles[16] = 10;
	model->triangles[17] = 11;

	//left
	model->vertices[12].homoCoord = HomoPoint3(-base, base, -base);
	model->vertices[13].homoCoord = HomoPoint3(-base, base, base);
	model->vertices[14].homoCoord = HomoPoint3(-base, -base, base);
	model->vertices[15].homoCoord = HomoPoint3(-base, -base, -base);

	model->vertices[12].uv = MyVector2(0, 0);
	model->vertices[13].uv = MyVector2(1, 0);
	model->vertices[14].uv = MyVector2(1, 1);
	model->vertices[15].uv = MyVector2(0, 1);

	model->triangles[18] = 12;
	model->triangles[19] = 15;
	model->triangles[20] = 14;

	model->triangles[21] = 12;
	model->triangles[22] = 14;
	model->triangles[23] = 13;

	//bottom
	model->vertices[16].homoCoord = HomoPoint3(-base, -base, base);
	model->vertices[17].homoCoord = HomoPoint3(base, -base, base);
	model->vertices[18].homoCoord = HomoPoint3(-base, -base, -base);
	model->vertices[19].homoCoord = HomoPoint3(base, -base, -base);

	model->vertices[16].uv = MyVector2(0, 0);
	model->vertices[17].uv = MyVector2(1, 0);
	model->vertices[18].uv = MyVector2(0, 1);
	model->vertices[19].uv = MyVector2(1, 1);

	model->triangles[24] = 16;
	model->triangles[25] = 18;
	model->triangles[26] = 19;

	model->triangles[27] = 16;
	model->triangles[28] = 19;
	model->triangles[29] = 17;

	//back
	model->vertices[20].homoCoord = HomoPoint3(base, base, -base);
	model->vertices[21].homoCoord = HomoPoint3(-base, base, -base);
	model->vertices[22].homoCoord = HomoPoint3(-base, -base, -base);
	model->vertices[23].homoCoord = HomoPoint3(base, -base, -base);

	model->vertices[20].uv = MyVector2(0, 0);
	model->vertices[21].uv = MyVector2(1, 0);
	model->vertices[22].uv = MyVector2(1, 1);
	model->vertices[23].uv = MyVector2(0, 1);

	model->triangles[30] = 20;
	model->triangles[31] = 23;
	model->triangles[32] = 22;

	model->triangles[33] = 20;
	model->triangles[34] = 22;
	model->triangles[35] = 21;
}

Scene::~Scene()
{
	delete model;
}

void Scene::Render(Canvas* pCanvas) {
	model->transform.position = MyVector3(px, py, pz);
	model->transform.rotation.EulerAngles(rx, ry, rz);
	this->camera.RenderModel(pCanvas, model);
}

