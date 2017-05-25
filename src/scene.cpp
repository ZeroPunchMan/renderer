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
	v0.homoCoord = HomoPoint3(-size, size, -depth);
	v1.homoCoord = HomoPoint3(size, size, -depth);
	v2.homoCoord = HomoPoint3(size, -size, -depth);
	v3.homoCoord = HomoPoint3(-size, -size, -depth);

	v0.uv = MyVector2(0, 0);
	v1.uv = MyVector2(1, 0);
	v2.uv = MyVector2(1, 1);
	v3.uv = MyVector2(0, 1);

	DrawTriangle(pCanvas, v0, v1, v3);
	DrawTriangle(pCanvas, v1, v2, v3);
	MyLog("render");
}

HomoPoint3 Scene::WorldToCamera(HomoPoint3 point) {
	//MyLog(this->camera.transform.position.ToString().c_str());
	HomoPoint3 newPoint;
	newPoint = point - this->camera.transform.position;
	newPoint = this->camera.transform.rotation.GetInverse() * newPoint;
	/*newPoint = this->camera.PerspectiveProj(newPoint);
	newPoint.Homogenize();*/
	return newPoint;
}

HomoPoint3 Scene::CameraToNDC(HomoPoint3 point) {
	HomoPoint3 newPoint = this->camera.PerspectiveProj(point);
	newPoint.pos /= newPoint.w;
	//newPoint.Homogenize();
	return newPoint;
}

void Scene::DrawTriangle(Canvas* pCanvas, Vertex v0, Vertex v1, Vertex v2) {
	v0.homoCoord = this->WorldToCamera(v0.homoCoord);
	v1.homoCoord = this->WorldToCamera(v1.homoCoord);
	v2.homoCoord = this->WorldToCamera(v2.homoCoord);

	Vertex max, mid, min; 
	if (v0.homoCoord.pos.z > v1.homoCoord.pos.z) {
		max = v0;
		min = v1;
	}
	else {
		max = v1;
		min = v0;
	}

	if (v2.homoCoord.pos.z > max.homoCoord.pos.z) {
		mid = max;
		max = v2;
	}
	else if (v2.homoCoord.pos.z <= min.homoCoord.pos.z) {
		mid = min;
		min = v2;
	}
	else {
		mid = v2;
	}

	if (max.homoCoord.pos.z <= this->camera.n) {
		//全部渲染
		v0.homoCoord = this->CameraToNDC(v0.homoCoord);
		v1.homoCoord = this->CameraToNDC(v1.homoCoord);
		v2.homoCoord = this->CameraToNDC(v2.homoCoord);
		pCanvas->DrawTriangle(&v0, &v1, &v2);
		return;
	}
	else if (min.homoCoord.pos.z >= this->camera.n) {
		//剔除
		return;
	}

	//部分渲染
	
	if (mid.homoCoord.pos.z >= this->camera.n) {
		//截取一个三角形并渲染
		Vertex vn, vm;
		double rate = (camera.n - min.homoCoord.pos.z) / (max.homoCoord.pos.z - min.homoCoord.pos.z);
		vn.homoCoord = max.homoCoord * rate + min.homoCoord * (1 - rate);
		vn.uv = max.uv * rate + min.uv * (1 - rate);

		rate = (camera.n - min.homoCoord.pos.z) / (mid.homoCoord.pos.z - min.homoCoord.pos.z);
		vm.homoCoord = mid.homoCoord * rate + min.homoCoord * (1 - rate);
		vm.uv = mid.uv * rate + min.uv * (1 - rate);

		min.homoCoord = this->CameraToNDC(min.homoCoord);
		vn.homoCoord = this->CameraToNDC(vn.homoCoord);
		vm.homoCoord = this->CameraToNDC(vm.homoCoord);
		pCanvas->DrawTriangle(&min, &vn, &vm);
	}
	else {
		//截取两个三角形并渲染
		Vertex vn, vm;
		double rate = (camera.n - min.homoCoord.pos.z) / (max.homoCoord.pos.z - min.homoCoord.pos.z);
		vn.homoCoord = max.homoCoord * rate + min.homoCoord * (1 - rate);
		vn.uv = max.uv * rate + min.uv * (1 - rate);

		rate = (camera.n - mid.homoCoord.pos.z) / (max.homoCoord.pos.z - mid.homoCoord.pos.z);
		vm.homoCoord = max.homoCoord * rate + mid.homoCoord * (1 - rate);
		vm.uv = max.uv * rate + mid.uv * (1 - rate);

		min.homoCoord = this->CameraToNDC(min.homoCoord);
		mid.homoCoord = this->CameraToNDC(mid.homoCoord);
		vn.homoCoord = this->CameraToNDC(vn.homoCoord);
		vm.homoCoord = this->CameraToNDC(vm.homoCoord);
		pCanvas->DrawTriangle(&min, &vn, &mid);
		pCanvas->DrawTriangle(&mid, &vn, &vm);
	}

}

