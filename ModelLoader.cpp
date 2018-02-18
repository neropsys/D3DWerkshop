#include "stdafx.h"
#include "ModelLoader.h"
#include <cassert>
#include "Vertex.h"
ModelLoader::ModelLoader() :
	mpSdkManager(nullptr),
	mpImporter(nullptr),
	mpScene(nullptr)
{
	Init();
}


ModelLoader::~ModelLoader()
{
	mpImporter->Destroy(true);
}

void ModelLoader::Init() {
	if (mpSdkManager)
		return;
	mpSdkManager = FbxManager::Create();
	auto ios = FbxIOSettings::Create(mpSdkManager, IOSROOT);
	mpSdkManager->SetIOSettings(ios);
	mpImporter = FbxImporter::Create(mpSdkManager, "");
	mpScene = FbxScene::Create(mpSdkManager, "");

}

bool ModelLoader::Load(const char* path)
{
	bool ret = mpImporter->Initialize(path, -1, mpSdkManager->GetIOSettings());
	assert(ret);

	ret = mpImporter->Import(mpScene);
	assert(ret);


	//FbxAxisSystem::DirectX.ConvertScene(mpScene);// directXAxis(FbxAxisSystem::EUpVector::eYAxis, FbxAxisSystem::EFrontVector::eParityEven, FbxAxisSystem::eLeftHanded);

	FbxAxisSystem directX = FbxAxisSystem::DirectX;
	auto sceneAxis = mpScene->GetGlobalSettings().GetAxisSystem();

	bool isDxAxis = true;
	if (directX != sceneAxis)
	{
		isDxAxis = false;
	}// todo: need switch case for different axis

	auto rootNode = mpScene->GetRootNode();
	assert(rootNode);

	for (int i = 0; i < rootNode->GetChildCount(); i++)
	{
		auto childNode = rootNode->GetChild(i);

		if (childNode->GetNodeAttribute() == 0)
			continue;

		auto attribType = childNode->GetNodeAttribute()->GetAttributeType();
		if (attribType != FbxNodeAttribute::eMesh)
			continue;
		FbxGeometryConverter converter(mpSdkManager);

		auto mesh = (FbxMesh*)converter.Triangulate(childNode->GetNodeAttribute(), false);


		mIndexCount = mesh->GetPolygonVertexCount();
		for (int i = 0; i < mIndexCount; i++)
		{
			mIndices.push_back(i);
		}
		FbxVector4* vert = mesh->GetControlPoints();
		size_t polygonCount = mesh->GetPolygonCount();

		mVertices.reserve(polygonCount);
		for (size_t polygon = 0; polygon < polygonCount; polygon++)
		{
			int numVert = mesh->GetPolygonSize(polygon);
			for (int polygonVert = 0; polygonVert < numVert; polygonVert++)
			{
				int controlPtIndex = mesh->GetPolygonVertex(polygon, polygonVert);
				auto fbxVertex = vert[controlPtIndex].mData;
				Vertex vertex(fbxVertex[0], fbxVertex[1], fbxVertex[2]); // switch y and z to properly align
				if (isDxAxis == false)
				{
					vertex.pos = XMFLOAT3(fbxVertex[0], fbxVertex[2], fbxVertex[1]);
				}
				vertex.cpIndex = controlPtIndex;

				mVertices.emplace_back(vertex);

			}
		}
	}
// 	assert((mIndices.size() % 3) == 0);
// 	for (auto it = mIndices.begin(); it != mIndices.end(); it += 3)
// 	{
// 		std::swap(*it, *(it + 2));
// 	}

	return true;

}