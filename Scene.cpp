#include "stdafx.h"
#include "Scene.h"
#include "VertexLayout.h"
#include "DInput.h"
#include "Gizmo.h"
#include "imgui.h"
#include <string>
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
	using namespace D3D;
	Release(m_vertexShader);
	Release(m_pixelShader);
	Release(m_constantBuffer);
	Release(m_inputLayout);
	Release(m_standardInputLayout);
	Release(m_brdfVertexShader);
	Release(m_brdfPixelShader);
}

void Scene::Init()
{
	g_cam = std::make_unique<Camera>(1280, 720);
	HRESULT hr = S_OK;
	{
		ID3D10Blob* vertexBuffer = nullptr;
		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/VertexShader.cso", &vertexBuffer)));
		hr = D3D::device->CreateVertexShader(vertexBuffer->GetBufferPointer(), vertexBuffer->GetBufferSize(), NULL, &m_vertexShader);

		ID3D10Blob* pixelBuffer = nullptr;
		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/PixelShader.cso", &pixelBuffer)));
		hr = D3D::device->CreatePixelShader(pixelBuffer->GetBufferPointer(), pixelBuffer->GetBufferSize(), NULL, &m_pixelShader);

		UINT numElements = ARRAYSIZE(vertexLayout);
		hr = D3D::device->CreateInputLayout(vertexLayout, numElements, vertexBuffer->GetBufferPointer(), vertexBuffer->GetBufferSize(), &m_inputLayout);

		vertexBuffer->Release();
		pixelBuffer->Release();
	}
	{
		ID3D10Blob* vertexBuffer = nullptr;
	
		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/BRDFVertexShader.cso", &vertexBuffer)));
		auto hr = D3D::device->CreateVertexShader(vertexBuffer->GetBufferPointer(), vertexBuffer->GetBufferSize(), NULL, &m_brdfVertexShader);


		ID3D10Blob* pixelBuffer = nullptr;
		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/BRDFPixelShader.cso", &pixelBuffer)));
		hr = D3D::device->CreatePixelShader(pixelBuffer->GetBufferPointer(), pixelBuffer->GetBufferSize(), NULL, &m_brdfPixelShader);
		
		UINT numElements = ARRAYSIZE(vertexLayout_textured);
		hr = D3D::device->CreateInputLayout(vertexLayout_textured, numElements, vertexBuffer->GetBufferPointer(), vertexBuffer->GetBufferSize(), &m_standardInputLayout);

		vertexBuffer->Release();
		pixelBuffer->Release();
	}

	//model.
	auto model = std::make_unique<Model>("lantern\\lantern_obj.obj");
	model->PreRenderState([&]()
	{
		D3D::deviceContext->IASetInputLayout(m_standardInputLayout);
		D3D::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D::deviceContext->VSSetShader(m_brdfVertexShader, 0, 0);
		D3D::deviceContext->PSSetShader(m_brdfPixelShader, 0, 0);
	});

	model->SetWireFrame(false);
	m_models.emplace_back(std::move(model));


	auto gizmo = std::make_unique<Gizmo>();
	gizmo->PreRenderState([&]()
	{
		D3D::deviceContext->IASetInputLayout(m_inputLayout);
		D3D::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D::deviceContext->VSSetShader(m_vertexShader, 0, 0);
		D3D::deviceContext->PSSetShader(m_pixelShader, 0, 0);
	});
	m_models.emplace_back(std::move(gizmo));
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
	
	for (const auto& it : m_models)
	{
		it->Draw();
	}

}

