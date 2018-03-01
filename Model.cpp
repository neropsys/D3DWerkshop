#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include "D3D.h"
#include "Utilities.h"
Model::Model(const char * fileName)
{

	Model::Model();
	char buffer[MAX_PATH];
	//ZeroMemory(buffer, MAX_PATH);
	GetModuleFileName(NULL, buffer, MAX_PATH);

	auto filePath = path::GetAbsPath(fileName);

	assert(mloader.Load(filePath.c_str()));

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) *  mloader.GetVertexList().size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = &mloader.GetVertexList().data()[0];

	auto hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_VBuffer);
	assert(SUCCEEDED(hr));

	m_stride = sizeof(Vertex);
	m_offset = 0;

	m_indexCount = mloader.GetIndexCount();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * mloader.GetIndexCount();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	int indexCount = mloader.GetIndexCount();
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = mloader.GetIndexList().data();

	hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &m_IBuffer);
	assert(SUCCEEDED(hr));

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	hr = D3D::device->CreateRasterizerState(&desc, &m_wireframe);
	assert(SUCCEEDED(hr));
	//D3D::deviceContext->IASetVertexBuffers(0, 1, &mVBuffer, &stride, &offset);
	//D3D::deviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.ByteWidth = sizeof(D3D::cbPerObject);
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &m_constantBuffer)));
}

Model::Model() :
	m_VBuffer(nullptr),
	m_IBuffer(nullptr),
	m_stride(0),
	m_offset(0),
	m_indexCount(0),
	m_wireframe(nullptr),
	m_setwireframe(false),
	m_constantBuffer(nullptr),
	m_world(XMMatrixIdentity())
{
	mloader.Init();
}


Model::~Model()
{
	using namespace D3D;
	Release(m_VBuffer);
	Release(m_IBuffer);
	Release(m_constantBuffer);
	Release(m_wireframe);
}

void Model::Update(float delta) const
{
	auto ret = m_world * m_viewProj;
	D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &XMMatrixTranspose(ret), 0, 0);
}

void Model::Draw() 
{

	
	D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	D3D::deviceContext->IASetVertexBuffers(0, 1, &m_VBuffer, &m_stride, &m_offset);
	D3D::deviceContext->IASetIndexBuffer(m_IBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (m_setwireframe)
	{
		D3D::deviceContext->RSSetState(m_wireframe);
	}

	D3D::deviceContext->DrawIndexed(m_indexCount, 0, 0);
	if (m_setwireframe)
	{
		D3D::deviceContext->RSSetState(0);
	}
}

void Model::SetViewProj(const DirectX::XMMATRIX& ref)
{
	m_viewProj = ref;
}

