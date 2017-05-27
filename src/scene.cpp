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
	//����
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

	//����
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
	RenderModel(pCanvas, model);
}

void Scene::RenderModel(Canvas* pCanvas, Model *pModel) {//ģ�������������
	for (int i = 0; i < pModel->triCount * 3; i+= 3) {
		Vertex vertices[3];
		vertices[0] = pModel->vertices[pModel->triangles[i]];
		vertices[1] = pModel->vertices[pModel->triangles[i + 1]];
		vertices[2] = pModel->vertices[pModel->triangles[i + 2]];
		for (int k = 0; k < 3; k++) {
			//������ת
			vertices[k].homoCoord = pModel->transform.rotation.GetInverse() * vertices[k].homoCoord;
			//����ƫ�Ƶ���������
			vertices[k].homoCoord += pModel->transform.position;
		}
		//��������
		DrawTriangle(pCanvas, vertices[0], vertices[1], vertices[2]);
	}
}

HomoPoint3 Scene::WorldToCamera(HomoPoint3 point) {
	HomoPoint3 newPoint;
	newPoint = point - this->camera.transform.position;
	newPoint = this->camera.transform.rotation.GetInverse() * newPoint;
	return newPoint;
}

void Scene::DrawTriangle(Canvas* pCanvas, Vertex v0, Vertex v1, Vertex v2) {
	//�����޳�
	if (BackFaceCull(&v0, &v1, &v2))
		return;

	//�任�����������ϵ
	v0.homoCoord = this->WorldToCamera(v0.homoCoord);
	v1.homoCoord = this->WorldToCamera(v1.homoCoord);
	v2.homoCoord = this->WorldToCamera(v2.homoCoord);

	//frustum�޳�
	if (FrustumCull(&v0, &v1, &v2))
		return;

	//͸�Ӿ���任
	v0.homoCoord = this->camera.PerspectiveProj(v0.homoCoord);
	v1.homoCoord = this->camera.PerspectiveProj(v1.homoCoord);
	v2.homoCoord = this->camera.PerspectiveProj(v2.homoCoord);

	//�������ü�
	vector<Vertex> unclip;
	unclip.push_back(v0);
	unclip.push_back(v1);
	unclip.push_back(v2);
	vector<Vertex> clipped;
	this->HomoClip(&unclip, &clipped);
	if (clipped.size() == 0) {
		return;
	}

	//͸�ӳ���
	vector<Vertex>::iterator it;
	for (it = clipped.begin(); it != clipped.end(); it++) {
		it->homoCoord.pos /= it->homoCoord.w;
	}

	//���ǻ�
	vector<int> triangles;
	this->Triangulate(&clipped, &triangles);

	//�ӿ�������Ⱦ
	int count = triangles.size() / 3;
	for (int i = 0; i < count; i++) {
		int a, b, c;
		a = triangles[i * 3];
		b = triangles[i * 3 + 1];
		c = triangles[i * 3 + 2];
		pCanvas->DrawTriangle(&clipped[a], &clipped[b], &clipped[c]);
	}
}

bool Scene::BackFaceCull(Vertex *v0, Vertex *v1, Vertex *v2) {
	MyVector3 a = v1->homoCoord.pos - v0->homoCoord.pos;
	MyVector3 b = v2->homoCoord.pos - v0->homoCoord.pos;
	MyVector3 n = MyVector3::Cross(a, b);
	double res = MyVector3::Dot(n, v0->homoCoord.pos - this->camera.transform.position);
	return res >= 0;
}

bool Scene::FrustumCull(Vertex *pV0, Vertex *pV1, Vertex *pV2) {
	//cull������,����Ķ����ԭ��һ��
	uint8_t codes[3];
	Vertex* vertices[3];
	vertices[0] = pV0;
	vertices[1] = pV1;
	vertices[2] = pV2;
	codes[0] = 0;
	codes[1] = 0;
	codes[2] = 0;
	for (int i = 0; i < 3; i++) {
		//left
		if (vertices[i]->homoCoord.pos.x <= (this->camera.leftBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1;
		//right
		if (vertices[i]->homoCoord.pos.x >= (this->camera.rightBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 1;
		//near
		if(vertices[i]->homoCoord.pos.z >= this->camera.n)
			codes[i] |= 1 << 2;
		//far
		if (vertices[i]->homoCoord.pos.z <= this->camera.f)
			codes[i] |= 1 << 3;
		//top
		if(vertices[i]->homoCoord.pos.y >= (this->camera.topBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 4;
		//bottom
		if (vertices[i]->homoCoord.pos.y <= (this->camera.bottomBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 5;
	}
	uint8_t res = codes[0] & codes[1] & codes[2];
	return res != 0;
}

void Scene::Triangulate(vector<Vertex> *in, vector<int> *out) {
	//ƽ��ü�����ü����������,�ٶ�Ϊ͹����ν��д���
	for (int i = 1; i < in->size() - 1; i++) {
		out->push_back(0);
		out->push_back(i);
		out->push_back(i + 1);
	}
}

void Scene::HomoClip(vector<Vertex> *in, vector<Vertex> *out) {
	/*vector<Vertex>::iterator it;
	for (it = in->begin(); it != in->end(); it++) {
		out->push_back(*it);
	}*/
	//MyLog("clip");
	vector<Vertex> temp1, temp2;
	//��ƽ��ü�
	Clip(in, &temp1, VertexInNear, IntersectionOnNear);
	//Զƽ��ü�
	Clip(&temp1, &temp2, VertexInFar, IntersectionOnFar);
	//left�ü�
	Clip(&temp2, &temp1, VertexInLeft, IntersectionOnLeft);
	//right�ü�
	Clip(&temp1, &temp2, VertexInRight, IntersectionOnRight);
	//top�ü�
	Clip(&temp2, &temp1, VertexInTop, IntersectionOnTop);
	//bottom�ü�
	Clip(&temp1, &temp2, VertexInBottom, IntersectionOnBottom);

	out->clear();
	for (int i = 0; i < temp2.size(); i++) {
		out->push_back(temp2[i]);
	}
}

void Scene::Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection) {
	out->clear();
	
	Vertex* lastVertex = &(*(in->end()-1)); //����Ϊ���һ������
	bool lastInternal = isInternal(lastVertex, &camera); //��һ�����Ƿ����ڲ�
	if (lastInternal) {
		out->push_back(*lastVertex);
	}
	else {

	}
	vector<Vertex>::iterator it;
	for (it = in->begin(); it != in->end(); ++it) {
		bool curInternal = isInternal(&(*it), &camera);
		if (curInternal) { //��ǰ�����ڲ�
			if (lastInternal) { //��һ����Ҳ���ڲ�,ֱ��push������
				out->push_back(*it);
			}
			else { //��һ�������ⲿ,��ȡ����,��push����͵�ǰ��
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, &camera);
				out->push_back(intsct);
				out->push_back(*it);
			}
			
		}
		else { //��ǰ�����ⲿ
			if (lastInternal) { //��һ�������ڲ�,��ȡ���㲢push
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, &camera);
				out->push_back(intsct);
			}
			else { //��һ����Ҳ���ⲿ,����push�κε�

			}
		}

		lastVertex = &(*it); //��¼�µ�ǰ��,��Ϊ��һ�ε�����"��һ����"
		lastInternal = curInternal;
	}
}

//��ƽ��ü�
bool Scene::VertexInNear(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w <= pCamera->n;
}

void Scene::IntersectionOnNear(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("near clip");
	double t = (pCamera->n - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//Զƽ��ü�
bool Scene::VertexInFar(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w >= pCamera->f;
}

void Scene::IntersectionOnFar(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("far clip");
	double t = (pCamera->f - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//��ƽ��ü�
bool Scene::VertexInLeft(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.x <= -pV->homoCoord.w;
}

void Scene::IntersectionOnLeft(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("left clip");
	double t = (v2->homoCoord.w + v2->homoCoord.pos.x) /
		(v2->homoCoord.pos.x + v2->homoCoord.w - v1->homoCoord.pos.x - v1->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//right�ü�
bool Scene::VertexInRight(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.x >= pV->homoCoord.w;
}

void Scene::IntersectionOnRight(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("right clip");
	double t = (v2->homoCoord.w - v2->homoCoord.pos.x) /
		(v1->homoCoord.pos.x - v2->homoCoord.pos.x - v1->homoCoord.w + v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//��ƽ��ü�
bool Scene::VertexInTop(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.y <= -pV->homoCoord.w;
}
void Scene::IntersectionOnTop(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("top clip");
	double t = (v2->homoCoord.w + v2->homoCoord.pos.y) /
		(v2->homoCoord.pos.y + v2->homoCoord.w - v1->homoCoord.pos.y - v1->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//��ƽ��ü�
bool Scene::VertexInBottom(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.y >= pV->homoCoord.w;
}

void Scene::IntersectionOnBottom(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("bottom clip");
	double t = (v2->homoCoord.w - v2->homoCoord.pos.y) /
		(v1->homoCoord.pos.y - v2->homoCoord.pos.y - v1->homoCoord.w + v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}