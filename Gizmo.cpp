#include "stdafx.h"
#include "Gizmo.h"
#include "Vertex.h"
#include <vector>
#include "D3D.h"
Gizmo::Gizmo():m_stride(sizeof(Vertex)), m_offset(0)
{
	using namespace DirectX;
	float cylinderHeight = offset + m_magnitude * .8f;
	float cylinderRadius = m_magnitude * .007f;
	
	XMFLOAT3 cylinderPos[36];

	m_vertice.reserve(256);
	for (int i = 0; i < 256; i++)
	{
			m_vertice.emplace_back(Vertex(0, 0, 0));
	}
	{
		for (int i = 0; i < 18; i++)
		{
			int index = i * 2;
			int index2 = i * 2 + 1;
			float x = cylinderRadius * sin(2 * M_PI * (i / 17.f));
			float y = cylinderRadius * cos(2 * M_PI * (i / 17.f));
			cylinderPos[index] = XMFLOAT3(x, y, offset);
			cylinderPos[index2] = XMFLOAT3(x, y, cylinderHeight);
		}
		int indexOffset[6] = { 0, 2, 1, 1, 2, 3 };
		for (int i = 0; i < 17; i++)
		{
			for (int n = 0; n < 6; n++)
			{
				int index = i * 6 + n;
				int positionIndex = i * 2 + indexOffset[n];

				if (positionIndex >= 34)
					positionIndex -= 34;

				m_vertice[index].pos.x = cylinderPos[positionIndex].x;
				m_vertice[index].pos.y = cylinderPos[positionIndex].y;
				m_vertice[index].pos.z = cylinderPos[positionIndex].z;
				m_vertice[index].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
			}
		}

		float coneOffsetY = cylinderHeight;
		float coneHeight = m_magnitude - cylinderHeight + offset;
		float coneRadius = m_magnitude * .05;

		XMFLOAT3 conePos[18];

		for (int i = 0; i < 18; ++i)
		{
			float x = coneRadius * sin(2 * M_PI * (i / 17.f));
			float y = coneRadius * cos(2 * M_PI * (i / 17.f));
			conePos[i] = XMFLOAT3(x, y, coneOffsetY);
		}

		XMFLOAT3 coneTop(0, 0, coneHeight + coneOffsetY);
		XMFLOAT3 coneBottom(0, 0, coneOffsetY);

		for (int i = 0; i < 17; i++)
		{
			int index = i * 6 + 108;
			int positionIndex1 = i;
			int positionIndex2 = i + 1;

			m_vertice[index].pos.x = conePos[positionIndex2].x;
			m_vertice[index].pos.y = conePos[positionIndex2].y;
			m_vertice[index].pos.z = conePos[positionIndex2].z;
			m_vertice[index].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

			m_vertice[index + 1].pos.x = conePos[positionIndex1].x;
			m_vertice[index + 1].pos.y = conePos[positionIndex1].y;
			m_vertice[index + 1].pos.z = conePos[positionIndex1].z;
			m_vertice[index + 1].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

			m_vertice[index + 2].pos.x = coneTop.x;
			m_vertice[index + 2].pos.y = coneTop.y;
			m_vertice[index + 2].pos.z = coneTop.z;
			m_vertice[index + 2].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

			m_vertice[index + 3].pos.x = conePos[positionIndex1].x;
			m_vertice[index + 3].pos.y = conePos[positionIndex1].y;
			m_vertice[index + 3].pos.z = conePos[positionIndex1].z;
			m_vertice[index + 3].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

			m_vertice[index + 4].pos.x = conePos[positionIndex2].x;
			m_vertice[index + 4].pos.y = conePos[positionIndex2].y;
			m_vertice[index + 4].pos.z = conePos[positionIndex2].z;
			m_vertice[index + 4].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

			m_vertice[index + 5].pos.x = coneBottom.x;
			m_vertice[index + 5].pos.y = coneBottom.y;
			m_vertice[index + 5].pos.z = coneBottom.z;
			m_vertice[index + 5].color = XMFLOAT4(1.f, 0.f, 0.f, 1.f);
		}
	}

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


	m_indice.reserve(216);
	for (int i = 0; i < 216; i++)
	{
		m_indice.emplace_back(i);
	}

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

}

Gizmo::~Gizmo()
{

}

void Gizmo::Update(float delta) const
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void Gizmo::Draw() const
{
	D3D::deviceContext->IASetVertexBuffers(0, 1, &mVBuffer, &m_stride, &m_offset);
	D3D::deviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::deviceContext->DrawIndexed(m_indice.size(), 0, 0);
	//throw std::logic_error("The method or operation is not implemented.");
}

