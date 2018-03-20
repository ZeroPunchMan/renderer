#include "stdafx.h"

#include "FbxModel.h"

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
		exit(1);
	}

	FbxModel* model = new FbxModel();
	model->pFbxScene = FbxScene::Create(pFbxManager, "myScene");
	if (!model->pFbxScene) {
		CLog("create fbx scene failed");
		exit(1);
	}

	if (!pImporter->Import(model->pFbxScene)) {
		CLog("import fbx scene failed");
		exit(1);
	}
	pImporter->Destroy();

	return model;
}