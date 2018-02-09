#include "stdafx.h"
#include "GraphicResources.h"
#include "Vertex.h"
#include <d3dcompiler.h>
#include "Camera.h"
#include "ModelLoader.h"
namespace Resources {

	ID3D11Buffer* g_constantBuffer;

	ID3D11Buffer* g_squareVertexBuffer = nullptr;
	ID3D11Buffer* g_indiceBuffer = nullptr;

	ID3D11VertexShader* g_vertexShader= nullptr;
	ID3D11PixelShader* g_pixelShader = nullptr;

	ID3D10Blob* g_vsBuffer = nullptr;
	ID3D10Blob* g_psBuffer = nullptr;

	ID3D11InputLayout* g_inputLayout = nullptr;

	ID3D11Buffer* g_triIndiceBuffer = nullptr;
	ID3D11Buffer* g_triVertexBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	void Release()
	{
		Release(g_vsBuffer);
		Release(g_psBuffer);
		Release(g_pixelShader);
		Release(g_vertexShader);
		Release(g_indiceBuffer);
		Release(g_squareVertexBuffer);
		Release(g_triIndiceBuffer);
		Release(g_triVertexBuffer);
		Release(g_constantBuffer);
	}
	UINT numElements = ARRAYSIZE(vertexLayout);
	static int indexCount = 0;
	void InitResources()
	{
		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/VertexShader.cso", &g_vsBuffer)));
		auto hr = D3D::device->CreateVertexShader(g_vsBuffer->GetBufferPointer(), g_vsBuffer->GetBufferSize(), NULL, &g_vertexShader);

		assert(SUCCEEDED(D3DReadFileToBlob(L"./Debug/PixelShader.cso", &g_psBuffer)));
		hr = D3D::device->CreatePixelShader(g_psBuffer->GetBufferPointer(), g_psBuffer->GetBufferSize(), NULL, &g_pixelShader);

		//constantbuffer
		{
			D3D11_BUFFER_DESC cbbd;
			ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

			cbbd.Usage = D3D11_USAGE_DEFAULT;
			cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbbd.ByteWidth = sizeof(XMMATRIX);
			cbbd.CPUAccessFlags = 0;
			cbbd.MiscFlags = 0;

			assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &g_constantBuffer)));
		}


		D3D::deviceContext->VSSetShader(g_vertexShader, 0, 0);
		D3D::deviceContext->PSSetShader(g_pixelShader, 0, 0);

		//sqare
		{
// 			Vertex v[] = {
// 				Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
// 				Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
// 				Vertex(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
// 				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
// 			};
// 			D3D11_BUFFER_DESC vertexBufferDesc;
// 			ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// 			vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
// 			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 			vertexBufferDesc.CPUAccessFlags = 0;
// 			vertexBufferDesc.MiscFlags = 0;
// 
// 			D3D11_SUBRESOURCE_DATA vertexBufferData;
// 			ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
// 
// 			vertexBufferData.pSysMem = v;
// 			hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_squareVertexBuffer);
// 			assert(SUCCEEDED(hr));
// 
// 			auto stride = sizeof(Vertex);
// 			UINT offset = 0;
// 
// 			D3D::deviceContext->IASetVertexBuffers(0, 1, &g_squareVertexBuffer, &stride, &offset);
// 
// 
// 			DWORD indices[] = {
// 				0, 1, 2,
// 				0, 2, 3,
// 			};
// 			D3D11_BUFFER_DESC indexBufferDesc;
// 			ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
// 			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// 			indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
// 			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
// 			indexBufferDesc.CPUAccessFlags = 0;
// 
// 			D3D11_SUBRESOURCE_DATA initData;
// 			initData.pSysMem = indices;
// 			hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &g_indiceBuffer);
// 			assert(SUCCEEDED(hr));
// 
// 			D3D::deviceContext->IASetIndexBuffer(g_indiceBuffer, DXGI_FORMAT_R32_UINT, 0);
// 
// 
// 			hr = D3D::device->CreateInputLayout(vertexLayout, numElements, g_vsBuffer->GetBufferPointer(), g_vsBuffer->GetBufferSize(), &g_inputLayout);
// 			assert(SUCCEEDED(hr));

			
		}
		//triangle
		{
			ModelLoader loader;
			loader.Init();
			loader.Load("cone.fbx");

			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(Vertex) *  loader.GetVertexList().size();
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;

		
			D3D11_SUBRESOURCE_DATA vertexBufferData;
			ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

			vertexBufferData.pSysMem = &loader.GetVertexList().data()[0];
			hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_triVertexBuffer);
			assert(SUCCEEDED(hr));

			auto stride = sizeof(Vertex);
			UINT offset = 0;

			
			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(int) * loader.GetIndexCount();
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;

			indexCount = loader.GetIndexCount();
			D3D11_SUBRESOURCE_DATA initData;
			ZeroMemory(&initData, sizeof(initData));
			initData.pSysMem = loader.GetIndexList().data();

			hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &g_triIndiceBuffer);
			assert(SUCCEEDED(hr));

			D3D::deviceContext->IASetIndexBuffer(g_triIndiceBuffer, DXGI_FORMAT_R32_UINT, 0);


			hr = D3D::device->CreateInputLayout(vertexLayout, numElements, g_vsBuffer->GetBufferPointer(), g_vsBuffer->GetBufferSize(), &g_inputLayout);

		}
		D3D::deviceContext->IASetInputLayout(g_inputLayout);
		D3D::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11RasterizerState* WireFrame;
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		hr = D3D::device->CreateRasterizerState(&wfdesc, &WireFrame);
		D3D::deviceContext->RSSetState(WireFrame);
		
	}

	void UpdateResources(const D3D::cbPerObject& wvp)
	{
		D3D::deviceContext->UpdateSubresource(g_constantBuffer, 0, NULL, &wvp, 0, 0);
		D3D::deviceContext->VSSetConstantBuffers(0, 1, &g_constantBuffer);
	}

	void DrawResources()
	{
		auto stride = sizeof(Vertex);
		UINT offset = 0;


		 D3D::deviceContext->IASetVertexBuffers(0, 1, &g_triVertexBuffer, &stride, &offset);
		 D3D::deviceContext->IASetIndexBuffer(g_triIndiceBuffer, DXGI_FORMAT_R32_UINT, 0);
		 D3D::deviceContext->DrawIndexed(indexCount, 0, 0);

		// D3D::deviceContext->IASetVertexBuffers(0, 1, &g_squareVertexBuffer, &stride, &offset);
		// D3D::deviceContext->IASetIndexBuffer(g_indiceBuffer, DXGI_FORMAT_R32_UINT, 0);
		// D3D::deviceContext->DrawIndexed(6, 0, 0);


	}


}

