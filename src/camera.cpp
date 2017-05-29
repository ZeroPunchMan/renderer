#include "stdafx.h"
#include "camera.h"

void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, Vertex* pVertex)
{
	FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

	pVertex->homoCoord.pos.x = pCtrlPoint[ctrlPointIndex].mData[0];
	pVertex->homoCoord.pos.y = pCtrlPoint[ctrlPointIndex].mData[1];
	pVertex->homoCoord.pos.z = pCtrlPoint[ctrlPointIndex].mData[2];
	pVertex->homoCoord.w = 1;
}

void Camera::RenderModel(Canvas* pCanvas, FbxModel *pModel) {//模型由三角形组成
	FbxNode* pRootNode = pModel->pFbxScene->GetRootNode();
	
	FbxMesh* pMesh = pRootNode->GetChild(0)->GetMesh();	
	int polygonCount = pMesh->GetPolygonCount();
	
	for (int i = 0; i < polygonCount; i++) {
		Vertex vertices[3];
		for (int k = 0; k < 3; k++) {
			int offset = pMesh->GetPolygonVertex(i, k);
			ReadVertex(pMesh, offset, &vertices[k]);
			vertices[k].homoCoord = pModel->transform.rotation * vertices[k].homoCoord;
			vertices[k].homoCoord += pModel->transform.position;
		}
		DrawTriangle(pCanvas, vertices[0], vertices[1], vertices[2]);
	}

	/*Vertex v[3];
	v[0].homoCoord.pos = MyVector3(0, 0, -50);
	v[1].homoCoord.pos = MyVector3(0, -50, -50);
	v[2].homoCoord.pos = MyVector3(50, 0, -50);
	for (int i = 0; i < 3; i++) {
	v[i].homoCoord = pModel->transform.rotation * v[i].homoCoord;
	v[i].homoCoord += pModel->transform.position;
	}
	DrawTriangle(pCanvas, v[0], v[1], v[2]);*/
}

HomoPoint3 Camera::WorldToCamera(HomoPoint3 point) {
	HomoPoint3 newPoint;
	newPoint = point - this->transform.position;
	newPoint = this->transform.rotation.GetInverse() * newPoint;
	return newPoint;
}

void Camera::DrawTriangle(Canvas* pCanvas, Vertex v0, Vertex v1, Vertex v2) {
	//背面剔除
	/*if (BackFaceCull(&v0, &v1, &v2))
		return;*/

	//变换到摄像机坐标系
	v0.homoCoord = this->WorldToCamera(v0.homoCoord);
	v1.homoCoord = this->WorldToCamera(v1.homoCoord);
	v2.homoCoord = this->WorldToCamera(v2.homoCoord);

	//frustum剔除
	if (FrustumCull(&v0, &v1, &v2))
		return;

	//透视矩阵变换
	v0.homoCoord = this->PerspectiveProj(v0.homoCoord);
	v1.homoCoord = this->PerspectiveProj(v1.homoCoord);
	v2.homoCoord = this->PerspectiveProj(v2.homoCoord);

	//齐次坐标裁剪
	vector<Vertex> unclip;
	unclip.push_back(v0);
	unclip.push_back(v1);
	unclip.push_back(v2);

	vector<Vertex> clipped;
	this->HomoClip(&unclip, &clipped);
	if (clipped.size() == 0) {
		return;
	}

	
	vector<Vertex>::iterator it;
	for (it = clipped.begin(); it != clipped.end(); it++) {
		//透视除法
		it->homoCoord.pos /= it->homoCoord.w;
	}

	//三角化
	vector<int> triangles;
	this->Triangulate(&clipped, &triangles);

	
	//视口坐标渲染三角形
	int count = triangles.size() / 3;
	for (int i = 0; i < count; i++) {
		int a, b, c;
		a = triangles[i * 3];
		b = triangles[i * 3 + 1];
		c = triangles[i * 3 + 2];
		pCanvas->DrawTriangle(&clipped[a], &clipped[b], &clipped[c]);

		/*pCanvas->LineBres(&clipped[a], &clipped[b], &lineColor);
		pCanvas->LineBres(&clipped[b], &clipped[c], &lineColor);
		pCanvas->LineBres(&clipped[c], &clipped[a], &lineColor);*/
	}

	//画mesh
	MyColor lineColor(0, 1, 1);
	for (vector<Vertex>::iterator it = clipped.begin(); it != clipped.end(); it++) {
		if (it != clipped.begin()) {
			pCanvas->LineBres(&(*it), &(*(it - 1)), &lineColor);
		}
		else {
			pCanvas->LineBres(&(*it), &(*(clipped.end() - 1)), &lineColor);
		}
	}
}

bool Camera::BackFaceCull(Vertex *v0, Vertex *v1, Vertex *v2) {
	MyVector3 a = v1->homoCoord.pos - v0->homoCoord.pos;
	MyVector3 b = v2->homoCoord.pos - v0->homoCoord.pos;
	MyVector3 n = MyVector3::Cross(a, b);
	double res = MyVector3::Dot(n, v0->homoCoord.pos - this->transform.position);
	return res >= 0;
}

bool Camera::FrustumCull(Vertex *pV0, Vertex *pV1, Vertex *pV2) {
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
		if (vertices[i]->homoCoord.pos.x <= (this->leftBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1;
		//right
		if (vertices[i]->homoCoord.pos.x >= (this->rightBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 1;
		//near
		if (vertices[i]->homoCoord.pos.z >= this->n)
			codes[i] |= 1 << 2;
		//far
		if (vertices[i]->homoCoord.pos.z <= this->f)
			codes[i] |= 1 << 3;
		//top
		if (vertices[i]->homoCoord.pos.y >= (this->topBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 4;
		//bottom
		if (vertices[i]->homoCoord.pos.y <= (this->bottomBd * vertices[i]->homoCoord.pos.z))
			codes[i] |= 1 << 5;
	}
	uint8_t res = codes[0] & codes[1] & codes[2];
	return res != 0;
}

void Camera::Triangulate(vector<Vertex> *in, vector<int> *out) {
	//平面裁剪不会裁剪出凹多边形,假定为凸多边形进行处理
	for (int i = 1; i < in->size() - 1; i++) {
		out->push_back(0);
		out->push_back(i);
		out->push_back(i + 1);
	}
}

void Camera::HomoClip(vector<Vertex> *in, vector<Vertex> *out) {
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

void Camera::Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection) {
	out->clear();

	Vertex* lastVertex = &(*(in->end() - 1)); //设置为最后一个顶点
	bool lastInternal = isInternal(lastVertex, this); //上一个点是否在内部
	if (lastInternal) {
		out->push_back(*lastVertex);
	}
	else {

	}
	vector<Vertex>::iterator it;
	for (it = in->begin(); it != in->end(); ++it) {
		bool curInternal = isInternal(&(*it), this);
		if (curInternal) { //当前点在内部
			if (lastInternal) { //上一个点也在内部,直接push就行了
				if(it != in->end()-1)
					out->push_back(*it);
			}
			else { //上一个点在外部,获取交点,并push交点和当前点
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, this);
				out->push_back(intsct);
				out->push_back(*it);
			}

		}
		else { //当前点在外部
			if (lastInternal) { //上一个点在内部,获取交点并push
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, this);
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
bool Camera::VertexInNear(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w <= pCamera->n;
}

void Camera::IntersectionOnNear(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("near clip");
	double t = (pCamera->n - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//远平面裁剪
bool Camera::VertexInFar(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w >= pCamera->f;
}

void Camera::IntersectionOnFar(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("far clip");
	double t = (pCamera->f - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//左平面裁剪
bool Camera::VertexInLeft(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.x <= -pV->homoCoord.w;
}

void Camera::IntersectionOnLeft(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("left clip");
	double t = (v2->homoCoord.w + v2->homoCoord.pos.x) /
		(v2->homoCoord.pos.x + v2->homoCoord.w - v1->homoCoord.pos.x - v1->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//right裁剪
bool Camera::VertexInRight(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.x >= pV->homoCoord.w;
}

void Camera::IntersectionOnRight(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("right clip");
	double t = (v2->homoCoord.w - v2->homoCoord.pos.x) /
		(v1->homoCoord.pos.x - v2->homoCoord.pos.x - v1->homoCoord.w + v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//上平面裁剪
bool Camera::VertexInTop(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.y <= -pV->homoCoord.w;
}
void Camera::IntersectionOnTop(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("top clip");
	double t = (v2->homoCoord.w + v2->homoCoord.pos.y) /
		(v2->homoCoord.pos.y + v2->homoCoord.w - v1->homoCoord.pos.y - v1->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//下平面裁剪
bool Camera::VertexInBottom(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.pos.y >= pV->homoCoord.w;
}

void Camera::IntersectionOnBottom(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("bottom clip");
	double t = (v2->homoCoord.w - v2->homoCoord.pos.y) /
		(v1->homoCoord.pos.y - v2->homoCoord.pos.y - v1->homoCoord.w + v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}