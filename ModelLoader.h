#pragma once
#include<fbxsdk.h>
#include <vector>
#include "Vertex.h"
#include <string>
class ModelLoader {
public:
	inline const std::vector<Vertex>& GetVertexList() const { return m_vertice; }
	inline const std::vector<UINT>& GetIndexList() const { return m_indice; }
	inline const int GetIndexCount() const { return m_indexCount; }
	ModelLoader();
	~ModelLoader();
	void Init();
	bool Load(const char* path);
protected:
	bool ParseFbx(const char* path);
	bool ParseObj(const char* path);
	std::vector<Vertex> m_vertice;
	std::vector<UINT> m_indice;
	std::vector<std::string> m_texturePaths;//map(key std::string(textureType), value std::string(texturePath)
	int m_indexCount;						//or load instantly and set as value
	FbxManager* m_pSdkMgr;
	FbxImporter* m_importer;
	FbxScene* m_scene;

};