#pragma once
#include<fbxsdk.h>
#include <vector>
#include <map>
#include "Vertex.h"
#include <string>
#include "DirectXTex.h"
class ModelLoader {
public:
	enum TextureType
	{
		DIFFUSE,
		METALLIC,
		AO,
		NORMAL,
		OPACITY,
		ROUGHNESS
	};
	inline const std::vector<StandardVertex>& GetVertexList() const { return m_standard_vertice; }
	inline const std::vector<UINT>& GetIndexList() const { return m_indice; }
	inline const int GetIndexCount() const { return m_indexCount; }
	inline const bool HasTexture() const { return m_textures.empty(); }
	inline ID3D11ShaderResourceView** GetTexture(TextureType type)
	{
		auto ret = m_textures.find(type);
		
		return ret == m_textures.end() ? nullptr : &ret->second;
	}
	ModelLoader();
	~ModelLoader();
	void Init();
	bool Load(const char* absPath, const char* basePath);
protected:
	bool ParseFbx(const char* path);
	bool ParseObj(const char* absPath, const char* basePath);
	DirectX::ScratchImage LoadTexture(const wchar_t* fileName);
	std::vector<StandardVertex> m_standard_vertice;
	std::vector<UINT> m_indice;
	std::map<TextureType, ID3D11ShaderResourceView*> m_textures;
	std::vector<std::string> m_texturePaths;
	int m_indexCount;						
	FbxManager* m_pSdkMgr;
	FbxImporter* m_importer;
	FbxScene* m_scene;

};