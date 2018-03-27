#include "stdafx.h"
#include "ModelLoader.h"
#include <cassert>
#include "Vertex.h"
#include <algorithm>
#include "D3D.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
ModelLoader::ModelLoader() :
	m_pSdkMgr(nullptr),
	m_importer(nullptr),
	m_scene(nullptr)
{
	Init();
}


ModelLoader::~ModelLoader()
{
	for (const auto& it : m_textures)
	{
		it.second->Release();
	}
	//m_pSdkMgr->Destroy();
	m_importer->Destroy(true);
}

void ModelLoader::Init() {
	if (m_pSdkMgr)
		return;
	m_pSdkMgr = FbxManager::Create();
	auto ios = FbxIOSettings::Create(m_pSdkMgr, IOSROOT);
	m_pSdkMgr->SetIOSettings(ios);
	m_importer = FbxImporter::Create(m_pSdkMgr, "");
	m_scene = FbxScene::Create(m_pSdkMgr, "");

}

bool ModelLoader::Load(const char* path, const char* basePath)
{
	if (strstr(basePath, ".fbx") != nullptr)
	{
		return ParseFbx(path);
	}
	else if (strstr(basePath, ".obj") != nullptr)
	{
		return ParseObj(path, basePath);
	}
	
// 	assert((mIndices.size() % 3) == 0);
// 	for (auto it = mIndices.begin(); it != mIndices.end(); it += 3)
// 	{
// 		std::swap(*it, *(it + 2));
// 	}

	return false;

}

bool ModelLoader::ParseFbx(const char* path)
{
	/*
	bool ret = m_importer->Initialize(path, -1, m_pSdkMgr->GetIOSettings());
	assert(ret);

	ret = m_importer->Import(m_scene);
	assert(ret);


	//FbxAxisSystem::DirectX.ConvertScene(mpScene);// directXAxis(FbxAxisSystem::EUpVector::eYAxis, FbxAxisSystem::EFrontVector::eParityEven, FbxAxisSystem::eLeftHanded);

	FbxAxisSystem directX = FbxAxisSystem::DirectX;
	auto sceneAxis = m_scene->GetGlobalSettings().GetAxisSystem();

	bool isDxAxis = true;
	if (directX != sceneAxis)
	{
		isDxAxis = false;
	}// todo: need switch case for different axis

	auto rootNode = m_scene->GetRootNode();
	assert(rootNode);

	for (int i = 0; i < rootNode->GetChildCount(); i++)
	{
		auto childNode = rootNode->GetChild(i);

		if (childNode->GetNodeAttribute() == 0)
			continue;

		auto attribType = childNode->GetNodeAttribute()->GetAttributeType();
		if (attribType != FbxNodeAttribute::eMesh)
			continue;
		FbxGeometryConverter converter(m_pSdkMgr);

		auto mesh = (FbxMesh*)converter.Triangulate(childNode->GetNodeAttribute(), false);

		FbxVector4* vert = mesh->GetControlPoints();
		size_t polygonCount = mesh->GetPolygonCount();
		FbxStringList lUVSetNameList;
		mesh->GetUVSetNames(lUVSetNameList);

		//vertex
		m_vertice.reserve(polygonCount);
		for (size_t polygonIdx = 0; polygonIdx < polygonCount; polygonIdx++)
		{
			int numVert = mesh->GetPolygonSize(polygonIdx);
			for (int vertIndex = 0; vertIndex < numVert; vertIndex++)
			{
				int controlPtIndex = mesh->GetPolygonVertex(polygonIdx, vertIndex);
				auto fbxVertex = vert[controlPtIndex].mData;
				Vertex vertex(fbxVertex[0], fbxVertex[1], fbxVertex[2]); // switch y and z to properly align
				if (isDxAxis == false)
				{
					vertex.pos = XMFLOAT3(fbxVertex[0], fbxVertex[2], fbxVertex[1]);
				}

				for (int uvIndex = 0; uvIndex < lUVSetNameList.GetCount(); uvIndex++)
				{
					auto uvName = lUVSetNameList.GetItemAt(uvIndex);
					//	
					FbxVector2 uvValue;
					bool unmapped;
					mesh->GetPolygonVertexUV(polygonIdx, vertIndex, uvName->mString, uvValue, unmapped);

					vertex.color.x = uvValue[0];
					vertex.color.y = uvValue[1];

				}


				m_vertice.emplace_back(vertex);

			}
		}


		//get texture files
		int matCount = childNode->GetMaterialCount();
		for (int j = 0; j < matCount; j++)
		{
			auto material = childNode->GetMaterial(j);
			for (auto prop = material->GetFirstProperty(); prop.IsValid(); prop = material->GetNextProperty(prop))
			{
				auto srcProperty = prop;// material->GetSrcProperty(prop);
				auto srcObjectCount = srcProperty.GetSrcObjectCount();
				for (int srcObjectIdx = 0; srcObjectIdx < srcObjectCount; srcObjectIdx++)
				{
					auto realObject = srcProperty.GetSrcObject(srcObjectIdx);
					const char* typeName = realObject->GetTypeName();
					auto srcObject = (FbxFileTexture*)srcProperty.GetSrcObject(srcObjectIdx);
					std::string texturePath = srcObject->GetFileName();
					auto textureElement = std::find(m_texturePaths.begin(), m_texturePaths.end(), texturePath);
					if (textureElement == m_texturePaths.end())
					{
						//load texture path
						m_texturePaths.emplace_back(srcObject->GetFileName());
					}
				}
			}
			//for getting specific type of material
			// 			auto diffProp = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			// 			int srcObjectCount = diffProp.GetSrcObjectCount();
			// 			for (int k = 0; k < srcObjectCount; k++)
			// 			{
			// 				auto srcObject = (FbxFileTexture*) diffProp.GetSrcObject(k);
			// 				std::string texturePath = srcObject->GetFileName();
			// 				auto textureElement = std::find(m_texturePaths.begin(), m_texturePaths.end(), texturePath);
			// 				if (textureElement == m_texturePaths.end())
			// 				{
			// 					m_texturePaths.emplace_back(srcObject->GetFileName());
			// 				}
			// 				
			// 			}
		}

		//get uv coord

		for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
		{
			//get lUVSetIndex-th uv set
			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
			const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);

			if (!lUVElement)
				continue;

			// only support mapping mode eByPolygonVertex and eByControlPoint
			if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
				lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
				return false;
			bool isControlPt = false;
			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				isControlPt = true;
			}

			//index array, where holds the index referenced to the uv data
			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
			const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

			//iterating through the data by polygon
			const int lPolyCount = mesh->GetPolygonCount();

			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						FbxVector2 lUVValue;

						//get the index of the current vertex in control points array
						int lPolyVertIndex = mesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						float x = lUVValue.mData[0];
						float y = lUVValue.mData[1];
						//m_vertice[lVertIndex].color.x = lUVValue[0];
						//m_vertice[lVertIndex].color.y = lUVValue[1];
						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
					}
				}
			}
			else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				int lPolyIndexCounter = 0;
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						if (lPolyIndexCounter < lIndexCount)
						{

							FbxVector2 lUVValue;
							bool unmapped = true;
							mesh->GetPolygonVertexUV(lPolyIndex, lVertIndex, lUVSetName, lUVValue, unmapped);



							//the UV index depends on the reference mode
							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
							float x = lUVValue.mData[0];
							//mesh->GetPolygonVertexUV()
							//User TODO:
							//Print out the value of UV(lUVValue) or log it to a file
							m_vertice[lPolyIndex].color.x = lUVValue.mData[0];
							m_vertice[lPolyIndex].color.y = lUVValue.mData[1];
							if (lIndexCount == 3)
							{
								m_vertice[lPolyIndex].color.z = lUVValue[2];
							}
							lPolyIndexCounter++;
						}
					}
				}
			}
		}


		m_indexCount = mesh->GetPolygonVertexCount();
		for (int i = 0; i < m_indexCount; i++)
		{
			m_indice.push_back(i);
		}

	}
	*/
	return true;

}

bool ModelLoader::ParseObj(const char* absPath, const char* basePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	std::string fileName = basePath;
	auto idx = fileName.find_last_of("\\");
	if(idx != std::string::npos)
	{
		fileName = fileName.substr(idx+1, fileName.size());
	}
	std::string absPathStr = absPath;
	std::string basePathStr = absPathStr.append("\\");
	absPathStr.append(fileName);
	//fullPath += "\\" + std::string(basePath);
	//std::string absPathStr = absPath;
	//auto idx = absPathStr.find_last_of("\\");
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, absPathStr.c_str(), basePathStr.c_str());
	assert(ret);
	size_t vertices = attrib.vertices.size() / 3;
	for (size_t v = 0; v < vertices; v++)
	{
		m_standard_vertice.emplace_back(
			attrib.vertices[3 * v],
			attrib.vertices[3 * v + 1],
			attrib.vertices[3 * v + 2]);
	}
	struct vec2
	{
		float x;
		float y;
		vec2(float _x, float _y) :x(_x), y(_y) {}
		vec2 operator - (vec2 rh)
		{
			return vec2(x - rh.x, y - rh.y);
		}
	};
	std::vector<vec2> uvContainer;
	for (size_t v = 0; v < attrib.texcoords.size() / 2; v++)
	{
		uvContainer.emplace_back(
			attrib.texcoords[2 * v],
			1 - attrib.texcoords[2 * v + 1]);
	}

	std::vector<XMFLOAT3> normalContainer;
	for (size_t v = 0; v < attrib.normals.size()/3; v++)
	{
		normalContainer.push_back({ attrib.normals[3 * v], attrib.normals[3*v+1], attrib.normals[3*v+2] });
	}

	for (size_t shape = 0; shape < shapes.size(); shape++)
	{
		for (size_t f = 0; f < shapes[shape].mesh.indices.size() / 3; f++)
		{
			tinyobj::index_t i0 = shapes[shape].mesh.indices[3 * f];
			tinyobj::index_t i1 = shapes[shape].mesh.indices[3 * f + 1];
			tinyobj::index_t i2 = shapes[shape].mesh.indices[3 * f + 2];
			m_indice.push_back(i0.vertex_index);
			m_indice.push_back(i1.vertex_index);
			m_indice.push_back(i2.vertex_index);
			auto texIndex = uvContainer[i0.texcoord_index];
			auto texIndex1 = uvContainer[i1.texcoord_index];
			auto texIndex2 = uvContainer[i2.texcoord_index];
			m_standard_vertice[i0.vertex_index].tex = { texIndex.x, texIndex.y };
			m_standard_vertice[i1.vertex_index].tex = { texIndex1.x, texIndex1.y };
			m_standard_vertice[i2.vertex_index].tex = {texIndex2.x, texIndex2.y};

			auto v0 = DirectX::XMLoadFloat3(&m_standard_vertice[i0.vertex_index].pos);
			auto v1 = DirectX::XMLoadFloat3(&m_standard_vertice[i1.vertex_index].pos);
			auto v2 = DirectX::XMLoadFloat3(&m_standard_vertice[i2.vertex_index].pos);


			DirectX::XMFLOAT3 deltaPos;
			DirectX::XMStoreFloat3(&deltaPos, v1 - v0);

			DirectX::XMFLOAT3 deltaPos2;
			DirectX::XMStoreFloat3(&deltaPos2, v2 - v0);


			auto deltaUV = texIndex1 - texIndex;
			auto deltaUV2 = texIndex2 - texIndex;

			float r = 1.f / (deltaUV.x * deltaUV2.y - deltaUV2.x * deltaUV.y);
			
			float tanX = r * (deltaUV2.y * deltaPos.x - deltaUV.y * deltaPos2.x);
			float tanY = r * (deltaUV2.y * deltaPos.y - deltaUV.y * deltaPos2.y);
			float tanZ = r * (deltaUV2.y * deltaPos.z - deltaUV.y * deltaPos2.z);
			DirectX::XMFLOAT3 tangent(tanX, tanY, tanZ);
			DirectX::XMVECTOR tangentVector = DirectX::XMLoadFloat3(&tangent);

			tangentVector = DirectX::XMVector3Normalize(tangentVector);
			DirectX::XMStoreFloat3(&tangent, tangentVector);

			m_standard_vertice[i0.vertex_index].tan= tangent;
			m_standard_vertice[i1.vertex_index].tan = tangent;
			m_standard_vertice[i2.vertex_index].tan = tangent;

			m_standard_vertice[i0.vertex_index].norm = normalContainer[i0.normal_index];
			m_standard_vertice[i1.vertex_index].norm = normalContainer[i1.normal_index];
			m_standard_vertice[i2.vertex_index].norm = normalContainer[i2.normal_index];
		}
	}

	m_indexCount = m_indice.size();

	std::string texturePath = absPath;
	texturePath =  texturePath.append("\\").append("textures");


	HANDLE dir;
	WIN32_FIND_DATA file_data;
	std::vector<std::string> fileList;
	if ((dir = FindFirstFile((texturePath + "/*").c_str(), &file_data)) != INVALID_HANDLE_VALUE)
	{
		
		do
		{
			const std::string file_name = file_data.cFileName;
			const std::string full_file_name = texturePath + "/" + file_name;
			const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (file_name[0] == '.')
				continue;

			if (is_directory)
				continue;
			if (file_name.find(".jpg") != std::string::npos ||
				file_name.find(".png") != std::string::npos ||
				file_name.find(".BMP") != std::string::npos)
			{
				fileList.push_back(full_file_name);
			}

		} while (FindNextFile(dir, &file_data));

		FindClose(dir);
	}

	if(fileList.empty()==false)
	{
		HRESULT hr;
		for(const auto& it : fileList)
		{
			std::wstring wideStr = std::wstring(it.begin(), it.end());
			
			DirectX::ScratchImage image = LoadTexture(wideStr.c_str());
			ID3D11ShaderResourceView* texture = nullptr;

			hr = DirectX::CreateShaderResourceView(
				D3D::device,
				image.GetImages(),
				image.GetImageCount(),
				image.GetMetadata(),
				&texture);
			assert(SUCCEEDED(hr));

			if (wideStr.find(L"iffuse") != wideStr.npos || wideStr.find(L"Base") != wideStr.npos)
			{
				m_textures[DIFFUSE] = texture;
			}
			else if(wideStr.find(L"etallic") != wideStr.npos)
			{
				m_textures[METALLIC] = texture;
			}
			else if (wideStr.find(L"AO") != wideStr.npos)
			{
				m_textures[AO] = texture;
			}
			else if (wideStr.find(L"ormal") != wideStr.npos)
			{
				m_textures[NORMAL] = texture;
			}
			else if (wideStr.find(L"pacity") != wideStr.npos)
			{
				m_textures[OPACITY] = texture;
			}
			else if (wideStr.find(L"oughness") != wideStr.npos)
			{
				m_textures[ROUGHNESS] = texture;
			}

			image.Release();
		}
	}

	return true;
}

DirectX::ScratchImage ModelLoader::LoadTexture(const wchar_t* fileName)
{
	HRESULT hr;
	wchar_t ext[_MAX_EXT];
	DirectX::ScratchImage ret;
	_wsplitpath_s(fileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	//todo:: switch case by file extension

	hr = DirectX::LoadFromWICFile(fileName, DirectX::WIC_FLAGS_NONE, nullptr, ret);
	assert(SUCCEEDED(hr));
	return ret;

}
