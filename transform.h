#pragma once

#include "rotation.h"

class Transform
{
public:
	Transform() {};
	~Transform() {};

//private:
	Rotation rotation;
	MyVector3 position;
};
