#pragma once

#include "rotation.h"

class Transform
{
public:
	Transform() {};
	~Transform() {};

//private:
	Rotation rotation;
	MyMath::Vector3 position;
};
