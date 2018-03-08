#include "stdafx.h"
#include "ModelLoader.h"
#include <cassert>
#include "Vertex.h"
#include <algorithm>
#include <wincodec.h>
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

bool ModelLoader::Load(const char* path)
{
	if (strstr(path, ".fbx") != nullptr)
	{
		return ParseFbx(path);
	}
	else if (strstr(path, ".obj") != nullptr)
	{
		return ParseObj(path);
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
	return true;
}

bool ModelLoader::ParseObj(const char* path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string strBuffer = buffer;
	auto pos = strBuffer.find_last_of("\\/");
	auto baseDir = strBuffer.substr(0, pos);
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, (baseDir+"\\lantern\\").c_str());
	assert(ret);

	size_t vertices = attrib.vertices.size() / 3;
	for (size_t v = 0; v < vertices; v++)
	{
		m_vertice.emplace_back(
			attrib.vertices[3 * v],
			attrib.vertices[3 * v + 1],
			attrib.vertices[3 * v + 2]);
	}
	struct vec2
	{
		float x;
		float y;
		vec2(float _x, float _y):x(_x), y(_y){}
	};
	std::vector<vec2> uvContainer;
	for (size_t v = 0; v < attrib.texcoords.size() / 2; v++)
	{
		uvContainer.emplace_back(
			attrib.texcoords[2 * v], 
			attrib.texcoords[2 * v + 1]);
	}
	for(size_t shape=0; shape < shapes.size(); shape++)
	{
		for (size_t f = 0; f < shapes[shape].mesh.indices.size()/3; f++)
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
			m_vertice[i0.vertex_index].setUV(texIndex.x, texIndex.y);
			m_vertice[i1.vertex_index].setUV(texIndex1.x, texIndex2.y);
			m_vertice[i2.vertex_index].setUV(texIndex1.x, texIndex2.y);
		}
	}


// 	std::vector<vec2> texCoord;
// 	size_t texes = attrib.texcoords.size() / 2;
// 	for (size_t v = 0; v < texes; v++)
// 	{
// 		
// 	}

	m_indexCount = m_indice.size();
	return true;
}
