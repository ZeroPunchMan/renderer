#include "stdafx.h"
#include "scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Render(Canvas* pCanvas) {
	//MyLog(camera.mat.ToString().c_str());
	Vertex v0, v1, v2, v3;

	double depth = 50;
	double size = depth * 0.3f;
	v0.homoCoord = HomoPoint3(-100, 100, -50);
	v1.homoCoord = HomoPoint3(-200, 0, -60);
	v2.homoCoord = HomoPoint3(100, 100, -70);

	v0.uv = MyVector2(0, 0);
	v1.uv = MyVector2(1, 0);
	v2.uv = MyVector2(1, 1);

	DrawTriangle(pCanvas, v0, v1, v2);
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