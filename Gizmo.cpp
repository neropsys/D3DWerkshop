#include "stdafx.h"
#include "Gizmo.h"
#include "Vertex.h"
#include <vector>
#include "D3D.h"
#include "ArrowMesh.h"
Gizmo::Gizmo():m_stride(sizeof(Vertex)), m_offset(0), m_world(XMMatrixIdentity()), m_viewProj(XMMatrixIdentity())
{
	using namespace DirectX;
	

	ArrowMesh xAxis(ArrowMesh::X);
	ArrowMesh yAxis(ArrowMesh::Y);
	ArrowMesh zAxis(ArrowMesh::Z);
	m_vertice.insert(m_vertice.end(), xAxis.GetVertice().begin(), xAxis.GetVertice().end());
	m_vertice.insert(std::end(m_vertice), std::begin(yAxis.GetVertice()), std::end(yAxis.GetVertice()));
	m_vertice.insert(std::end(m_vertice), std::begin(zAxis.GetVertice()), std::end(zAxis.GetVertice()));


	m_indice.insert(m_indice.end(), xAxis.GetIndice().begin(), xAxis.GetIndice().end());
	m_indice.insert(m_indice.end(), yAxis.GetIndice().begin(), yAxis.GetIndice().end());
	m_indice.insert(m_indice.end(), zAxis.GetIndice().begin(), zAxis.GetIndice().end());
	m_indiceSize = xAxis.GetIndice().size();
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertice.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = &m_vertice.data()[0];

	auto hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVBuffer);
	assert(SUCCEEDED(hr));



	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indice.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = m_indice.data();

	hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &mIBuffer);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.ByteWidth = sizeof(XMMATRIX);
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &m_constantBuffer)));
	//m_world *= XMMatrixRotationY(1.5708f);// * XMMatrixScaling(1, 1, 1);

}

Gizmo::~Gizmo()
{

}

void Gizmo::Update(float delta) const
{

}

void Gizmo::Draw()
{


	D3D::deviceContext->IASetVertexBuffers(0, 1, &mVBuffer, &m_stride, &m_offset);
	D3D::deviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	{//x axis
		auto wvp =  XMMatrixRotationY(XMConvertToRadians(90)) * m_viewProj;
		D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &XMMatrixTranspose(wvp), 0, 0);
		D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		D3D::deviceContext->DrawIndexed(m_indiceSize, 0, 0);

	}
	

	{//y axis
		auto wvp =  XMMatrixRotationX(XMConvertToRadians(-90)) * m_viewProj;
		D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &XMMatrixTranspose(wvp), 0, 0);
		//D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		D3D::deviceContext->DrawIndexed(m_indiceSize, m_indiceSize, 0);

	}
	{//z axis
		auto wvp =  m_viewProj;
		D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &XMMatrixTranspose(wvp), 0, 0);
	//	D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		D3D::deviceContext->DrawIndexed(m_indiceSize, m_indiceSize * 2, 0);
	}

	//D3D::deviceContext->DrawIndexed(m_indice.size(), 0, 0);
}

void Gizmo::SetViewProj(const DirectX::XMMATRIX& ref)
{
	m_viewProj = XMMATRIX(ref);
}
