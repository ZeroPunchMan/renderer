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
	Vertex v0, v1, v2;
	v0.color = MyColor(1, 0, 0);
	v1.color = MyColor(0, 1, 0);
	v2.color = MyColor(0, 0, 1);

	v0.homoCoord = HomoPoint3(-150, 150, -50);
	v1.homoCoord = HomoPoint3(150, 150, -500);
	v2.homoCoord = HomoPoint3(0, 130, -900);

	DrawTriangle(pCanvas, v0, v1, v2);
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
	newPoint.Homogenize();
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
		float rate = (camera.n - min.homoCoord.pos.z) / (max.homoCoord.pos.z - min.homoCoord.pos.z);
		vn.homoCoord = max.homoCoord * rate + min.homoCoord * (1 - rate);
		vn.color = max.color * rate + min.color * (1 - rate);

		rate = (camera.n - min.homoCoord.pos.z) / (mid.homoCoord.pos.z - min.homoCoord.pos.z);
		vm.homoCoord = mid.homoCoord * rate + min.homoCoord * (1 - rate);
		vm.color = mid.color * rate + min.color * (1 - rate);

		min.homoCoord = this->CameraToNDC(min.homoCoord);
		vn.homoCoord = this->CameraToNDC(vn.homoCoord);
		vm.homoCoord = this->CameraToNDC(vm.homoCoord);
		pCanvas->DrawTriangle(&min, &vn, &vm);
	}
	else {
		//截取两个三角形并渲染
		Vertex vn, vm;
		float rate = (camera.n - min.homoCoord.pos.z) / (max.homoCoord.pos.z - min.homoCoord.pos.z);
		vn.homoCoord = max.homoCoord * rate + min.homoCoord * (1 - rate);
		vn.color = max.color * rate + min.color * (1 - rate);

		rate = (camera.n - mid.homoCoord.pos.z) / (max.homoCoord.pos.z - mid.homoCoord.pos.z);
		vm.homoCoord = max.homoCoord * rate + mid.homoCoord * (1 - rate);
		vm.color = max.color * rate + mid.color * (1 - rate);

		min.homoCoord = this->CameraToNDC(min.homoCoord);
		mid.homoCoord = this->CameraToNDC(mid.homoCoord);
		vn.homoCoord = this->CameraToNDC(vn.homoCoord);
		vm.homoCoord = this->CameraToNDC(vm.homoCoord);
		pCanvas->DrawTriangle(&min, &vn, &mid);
		pCanvas->DrawTriangle(&mid, &vn, &vm);
	}

}

