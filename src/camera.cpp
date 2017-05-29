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

void Camera::RenderModel(Canvas* pCanvas, FbxModel *pModel) {//ģ�������������
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
	//�����޳�
	/*if (BackFaceCull(&v0, &v1, &v2))
		return;*/

	//�任�����������ϵ
	v0.homoCoord = this->WorldToCamera(v0.homoCoord);
	v1.homoCoord = this->WorldToCamera(v1.homoCoord);
	v2.homoCoord = this->WorldToCamera(v2.homoCoord);

	//frustum�޳�
	if (FrustumCull(&v0, &v1, &v2))
		return;

	//͸�Ӿ���任
	v0.homoCoord = this->PerspectiveProj(v0.homoCoord);
	v1.homoCoord = this->PerspectiveProj(v1.homoCoord);
	v2.homoCoord = this->PerspectiveProj(v2.homoCoord);

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

	
	vector<Vertex>::iterator it;
	for (it = clipped.begin(); it != clipped.end(); it++) {
		//͸�ӳ���
		it->homoCoord.pos /= it->homoCoord.w;
	}

	//���ǻ�
	vector<int> triangles;
	this->Triangulate(&clipped, &triangles);

	
	//�ӿ�������Ⱦ������
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

	//��mesh
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
	//ƽ��ü�����ü����������,�ٶ�Ϊ͹����ν��д���
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

void Camera::Clip(vector<Vertex> *in, vector<Vertex> *out, VertexInternalFunc isInternal, IntersectionFunc getIntersection) {
	out->clear();

	Vertex* lastVertex = &(*(in->end() - 1)); //����Ϊ���һ������
	bool lastInternal = isInternal(lastVertex, this); //��һ�����Ƿ����ڲ�
	if (lastInternal) {
		out->push_back(*lastVertex);
	}
	else {

	}
	vector<Vertex>::iterator it;
	for (it = in->begin(); it != in->end(); ++it) {
		bool curInternal = isInternal(&(*it), this);
		if (curInternal) { //��ǰ�����ڲ�
			if (lastInternal) { //��һ����Ҳ���ڲ�,ֱ��push������
				if(it != in->end()-1)
					out->push_back(*it);
			}
			else { //��һ�������ⲿ,��ȡ����,��push����͵�ǰ��
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, this);
				out->push_back(intsct);
				out->push_back(*it);
			}

		}
		else { //��ǰ�����ⲿ
			if (lastInternal) { //��һ�������ڲ�,��ȡ���㲢push
				Vertex intsct;
				getIntersection(lastVertex, &(*it), &intsct, this);
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
bool Camera::VertexInNear(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w <= pCamera->n;
}

void Camera::IntersectionOnNear(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("near clip");
	double t = (pCamera->n - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//Զƽ��ü�
bool Camera::VertexInFar(Vertex *pV, Camera *pCamera) {
	return pV->homoCoord.w >= pCamera->f;
}

void Camera::IntersectionOnFar(Vertex *v1, Vertex *v2, Vertex *out, Camera *pCamera) {
	//MyLog("far clip");
	double t = (pCamera->f - v2->homoCoord.w) / (v1->homoCoord.w - v2->homoCoord.w);
	out->homoCoord = v1->homoCoord * t + v2->homoCoord * (1 - t);
	out->uv = v1->uv * t + v2->uv * (1 - t);
}

//��ƽ��ü�
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

//right�ü�
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

//��ƽ��ü�
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

//��ƽ��ü�
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