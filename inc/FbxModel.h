#pragma once

#include "fbxsdk.h"
#include "transform.h"

class FbxModel
{
public:
	FbxModel(){}
	~FbxModel(){}

	Transform transform;
	FbxScene *pFbxScene = NULL;

	static FbxModel* ImportFbxModel(const char *path);
private:
	static FbxManager* pFbxManager;
	static void ManagerInit();
	
};


