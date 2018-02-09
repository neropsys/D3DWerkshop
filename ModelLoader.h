#pragma once
#include<fbxsdk.h>
#include <vector>
struct Vertex;
class ModelLoader {
public:
	inline const std::vector<Vertex>& GetVertexList() const { return mVertices; }
	inline const std::vector<UINT>& GetIndexList() const { return mIndices; }
	inline const int GetIndexCount() const { return mIndexCount; }
	ModelLoader();
	~ModelLoader();
	void Init();
	bool Load(const char* path);
protected:
	std::vector<Vertex> mVertices;
	std::vector<UINT> mIndices;
	int mIndexCount;
	FbxManager* mpSdkManager;
	FbxImporter* mpImporter;
	FbxScene* mpScene;

};