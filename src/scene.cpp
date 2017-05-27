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
	//背面剔除
	if (BackFaceCull(&v0, &v1, &v2))
		return;

	//变换到摄像机坐标系
	v0.homoCoord = this->WorldToCamera(v0.homoCoord);
	v1.homoCoord = this->WorldToCamera(v1.homoCoord);
	v2.homoCoord = this->WorldToCamera(v2.homoCoord);

	//frustum剔除
	if (FrustumCull(&v0, &v1, &v2))
		return;

	//透视矩阵变换
	v0.homoCoord = this->camera.PerspectiveProj(v0.homoCoord);
	v1.homoCoord = this->camera.PerspectiveProj(v1.homoCoord);
	v2.homoCoord = this->camera.PerspectiveProj(v2.homoCoord);

	//齐次坐标裁剪
	vector<Vertex> unclip;
	unclip.push_back(v0);
	unclip.push_back(v1);
	unclip.push_back(v2);
	vector<Vertex> clipped;
	this->HomoClip(&unclip, &clipped);

	//透视除法
	vector<Vertex>::iterator it;
	for (it = clipped.begin(); it != clipped.end(); it++) {
		it->homoCoord.pos /= it->homoCoord.w;
	}

	//三角化
	vector<int> triangles;
	this->Triangulate(&clipped, &triangles);

	//视口坐标渲染
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
	//cull三角形,处理的多边形原理一样
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
	//平面裁剪不会裁剪出凹多边形,假定为凸多边形进行处理
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
	//近平面裁剪
	Clip(in, &temp1, VertexInNear, IntersectionOnNear);
	//远平面裁剪
	Clip(&temp1, &temp2, VertexInFar, IntersectionOnFar);
	//left裁剪
	Clip(&temp2, &temp1, VertexInLeft, IntersectionOnLeft);
	//right裁剪
	Clip(&temp1, &temp2, VertexInRight, IntersectionOnRight);
	//top裁剪
	Clip(&temp2, &temp1, VertexInTop, IntersectionOnTop);
	//bottom裁剪
	Clip(&temp1, &temp2, VertexInBottom, IntersectionOnBottom);

	out->clear();
	for (int i = 0; i < temp2.size(); i++) {
		out->push_back(temp2[i]);
	}
}

void Scene::Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection) {
	out->clear();
	
	Vertex* lastVertex = &(*(in->end()-1)); //设置为最后一个顶点
	bool lastInternal = isInternal(lastVertex, &camera); //上一个点是否在内部
	if (lastInternal) {
		out->push_back(*lastVertex);
	}
	else {

	}
	vector<Vertex>::iterator it;
	for (it = in->begin(); it != in->end(); ++it) {
		bool curInternal = isInternal(&(*it), &camera);
		if (curInternal) { //当前点在内部
			if (lastInternal) { //上一个点也在内部,直接push就行了
				out->push_back(*it);
			}
			else { //上一个点在外部,获取交点,并push交点和当前点
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, &camera);
				out->push_back(intsct);
				out->push_back(*it);
			}
			
		}
		else { //当前点在外部
			if (lastInternal) { //上一个点在内部,获取交点并push
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, &camera);
				out->push_back(intsct);
			}
			else { //上一个点也在外部,不用push任何点

			}
		}

		lastVertex = &(*it); //记录下当前点,做为下一次迭代的"上一个点"
		lastInternal = curInternal;
	}
}

//近平面裁剪
bool Scene::VertexInNear(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w <= pCamera->n;
}

void Scene::IntersectionOnNear(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("near clip");
	double t = (pCamera->n - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//远平面裁剪
bool Scene::VertexInFar(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w >= pCamera->f;
}

void Scene::IntersectionOnFar(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("far clip");
	double t = (pCamera->f - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//左平面裁剪
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

//right裁剪
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

//上平面裁剪
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

//下平面裁剪
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