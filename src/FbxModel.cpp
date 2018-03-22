#include "stdafx.h"

#include "FbxModel.h"
#include "common.h"

FbxManager* FbxModel::pFbxManager = NULL;

void FbxModel::ManagerInit() {
	static bool initialized = false;
	if (initialized)
		return;

	pFbxManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(ios);

	initialized = true;
}

FbxModel* FbxModel::ImportFbxModel(const char *path) {
	ManagerInit();

	FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");
	if (!pImporter->Initialize(path, -1, FbxModel::pFbxManager->GetIOSettings())) {
		CLog("Call to FbxImporter::Initialize() failed.\n");
		CLog("Error returned: %s\n\n", pImporter->GetStatus().GetErrorString());
		ErrorBox("import fbx model failed");
	}

	FbxModel* model = new FbxModel();
	model->pFbxScene = FbxScene::Create(pFbxManager, "myScene");
	if (!model->pFbxScene) {
		CLog("create fbx scene failed");
		ErrorBox("import fbx model failed");
	}

	if (!pImporter->Import(model->pFbxScene)) {
		CLog("import fbx scene failed");
		ErrorBox("import fbx model failed");
	}
	pImporter->Destroy();

	return model;
}