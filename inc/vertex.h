#pragma once

#include "math3d.h"
#include "transform.h"

struct Vertex {
	MyMath::HomoVector4 homoCoord;
	MyMath::Vector2 uv;
	Vertex(){}
};

struct Model
{
	Model(){}
	~Model() {
		if (vertices != NULL) {
			delete vertices;
			vertices = NULL;
			verCount = 0;
		}
		if (triangles != NULL) {
			delete triangles;
			triangles = NULL;
			triCount = 0;
		}
	}
	Transform transform;
	Vertex *vertices = NULL;
	int verCount;
	int *triangles = NULL;
	int triCount;
};
