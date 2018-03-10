#pragma once
#include<fbxsdk.h>
#include <vector>
#include "Vertex.h"
#include <string>
#include "DirectXTex.h"
class ModelLoader {
public:
	inline const std::vector<Vertex>& GetVertexList() const { return m_vertice; }
	inline const std::vector<UINT>& GetIndexList() const { return m_indice; }
	inline const int GetIndexCount() const { return m_indexCount; }
	inline const std::vector<ID3D11ShaderResourceView*> GetTextureList() const { return m_textures; }
	ModelLoader();
	~ModelLoader();
	void Init();
	bool Load(const char* absPath, const char* basePath);
protected:
	bool ParseFbx(const char* path);
	bool ParseObj(const char* absPath, const char* basePath);
	DirectX::ScratchImage LoadTexture(const wchar_t* fileName);
	std::vector<Vertex> m_vertice;
	std::vector<UINT> m_indice;
	std::vector<ID3D11ShaderResourceView*> m_textures;
	std::vector<std::string> m_texturePaths;//map(key std::string(textureType), value std::string(texturePath)
	int m_indexCount;						//or load instantly and set as value
	FbxManager* m_pSdkMgr;
	FbxImporter* m_importer;
	FbxScene* m_scene;

};