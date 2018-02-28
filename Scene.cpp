#include "stdafx.h"
#include "Scene.h"
#include "VertexLayout.h"
#include "DInput.h"
#include "Gizmo.h"
#include "imgui.h"
namespace D3D {
	extern ID3D11Device* device;
	extern ID3D11DeviceContext* deviceContext;
}

Scene::Scene():
	m_constantBuffer(nullptr),
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_inputLayout(nullptr)
{

}

Scene::~Scene()
{
	
}

void Scene::Init()
{
	g_cam = std::make_unique<Camera>(1280, 720);
	ID3D10Blob* g_vsBuffer = nullptr;
	assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/VertexShader.cso", &g_vsBuffer)));
	auto hr = D3D::device->CreateVertexShader(g_vsBuffer->GetBufferPointer(), g_vsBuffer->GetBufferSize(), NULL, &m_vertexShader);

	ID3D10Blob* g_psBuffer = nullptr;
	assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/PixelShader.cso", &g_psBuffer)));
	hr = D3D::device->CreatePixelShader(g_psBuffer->GetBufferPointer(), g_psBuffer->GetBufferSize(), NULL, &m_pixelShader);

	UINT numElements = ARRAYSIZE(vertexLayout);
	hr = D3D::device->CreateInputLayout(vertexLayout, numElements, g_vsBuffer->GetBufferPointer(), g_vsBuffer->GetBufferSize(), &m_inputLayout);

	//model.
//	m_models.emplace_back(std::move(new Model("Knuckles_meme\\Knuckles.fbx")));
	m_models.emplace_back(std::move(new Gizmo()));



	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.ByteWidth = sizeof(DirectX::XMMATRIX);
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &m_constantBuffer)));
}

void Scene::Update(float delta)
{
		g_cam->Update(delta);


		for (const auto& it : m_models)
		{
			it->SetViewProj(g_cam->getViewProj());
			it->Update(delta);
		}
	
}

void Scene::Draw()
{
	D3D::deviceContext->IASetInputLayout(m_inputLayout);
	D3D::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::deviceContext->VSSetShader(m_vertexShader, 0, 0);
	D3D::deviceContext->PSSetShader(m_pixelShader, 0, 0);
	for (const auto& it : m_models)
	{
		it->Draw();
	}
	ImGui::Text("Hello, world!");

}

