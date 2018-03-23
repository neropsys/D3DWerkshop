#include "stdafx.h"
#include "Cube.h"
Cube::Cube(float size, XMFLOAT4 color) :
	Cube()
{
	Vertex cubeVertices[] =
	{
		// Front
		Vertex{ { -size / 2, -size / 2, size / 2 }, color },	// LBN,
		Vertex{ { size / 2, -size / 2, size / 2 }, color }, 	// RBN, 
		Vertex{ { size / 2, size / 2, size / 2 }, color }, 		// RTN,
		Vertex{ { size / 2, size / 2, size / 2 }, color },		// RTN, 
		Vertex{ { -size / 2, size / 2, size / 2 }, color },		// LTN, 
		Vertex{ { -size / 2, -size / 2, size / 2 }, color },	// LBN,
																// Top
		Vertex{ { -size / 2, size / 2, size / 2 }, color },		// LTN, 
		Vertex{ { size / 2, size / 2, size / 2 }, color },		// RTN, 
		Vertex{ { size / 2, size / 2, -size / 2 }, color },		// RTF,
		Vertex{ { size / 2, size / 2, -size / 2 }, color },		// RTF, 
		Vertex{ { -size / 2, size / 2, -size / 2 }, color },	// LTF, 
		Vertex{ { -size / 2, size / 2, size / 2 }, color },		// LTN,

																														// Back
		Vertex{ { size / 2, -size / 2, -size / 2 }, color },	// RBF, 
		Vertex{ { -size / 2, -size / 2, -size / 2 }, color },	// LBF, 
		Vertex{ { -size / 2, size / 2, -size / 2 }, color },	// LTF,
		Vertex{ { -size / 2, size / 2, -size / 2 }, color },	// LTF, 
		Vertex{ { size / 2, size / 2, -size / 2 }, color },		// RTF, 
		Vertex{ { size / 2, -size / 2, -size / 2 }, color },	// RBF,

																																												// Bottom
		Vertex{ { -size / 2, -size / 2, -size / 2 }, color },	// LBF, 
		Vertex{ { size / 2, -size / 2, -size / 2 }, color },	// RBF, 
		Vertex{ { size / 2, -size / 2, size / 2 }, color }, 	// RBN,
		Vertex{ { size / 2, -size / 2, size / 2 }, color }, 	// RBN, 
		Vertex{ { -size / 2, -size / 2, size / 2 }, color },	// LBN, 
		Vertex{ { -size / 2, -size / 2, -size / 2 }, color },	// LBF,
// Right
		Vertex{ { size / 2, -size / 2, size / 2 }, color }, 	// RBN, 
		Vertex{ { size / 2, -size / 2, -size / 2 }, color }, 	// RBF, 
		Vertex{ { size / 2, size / 2, -size / 2 }, color },		// RTF,
		Vertex{ { size / 2, size / 2, -size / 2 }, color },		// RTF, 
		Vertex{ { size / 2, size / 2, size / 2 }, color }, 		// RTN, 
		Vertex{ { size / 2, -size / 2, size / 2 }, color }, 	// RBN,
// Left
		Vertex{ { -size / 2, -size / 2, -size / 2 }, color },	// LBF, 
		Vertex{ { -size / 2, -size / 2, size / 2 }, color },	// LBN, 
		Vertex{ { -size / 2, size / 2, size / 2 }, color },		// LTN,
		Vertex{ { -size / 2, size / 2, size / 2 }, color },		// LTN, 
		Vertex{ { -size / 2, size / 2, -size / 2 }, color },	// LTF, 
		Vertex{ { -size / 2, -size / 2, -size / 2 }, color }	// LBF,
	};
	m_vertice.assign(cubeVertices, std::end(cubeVertices));
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


	auto hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_VBuffer);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < m_vertice.size(); i++)
	{
		m_indice.emplace_back(i);
	}
	m_indiceSize = m_indice.size();
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indice.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = m_indice.data();

	hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &m_IBuffer);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.ByteWidth = sizeof(XMMATRIX);
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &m_constantBuffer)));
}

Cube::Cube():m_stride(sizeof(Vertex)),
m_offset(0),
m_world(XMMatrixIdentity()),
m_viewProj(XMMatrixIdentity()),
m_preRenderState(nullptr),
m_VBuffer(nullptr),
m_IBuffer(nullptr),
m_constantBuffer(nullptr),
m_indiceSize(0)
{

}

Cube::~Cube()
{
	using namespace D3D;
	Release(m_VBuffer);
	Release(m_IBuffer);
	Release(m_constantBuffer);
}

void Cube::PreRenderState(std::function<void() > preState)
{
	m_preRenderState = preState;
}

void Cube::SetViewProj(const DirectX::XMMATRIX& ref)
{
	m_viewProj = XMMATRIX(ref);
}

void Cube::Update(float delta) const
{
	XMVECTOR vec;
	vec = XMLoadFloat3(&m_pos);

}

void Cube::Draw()
{
	if (m_preRenderState != nullptr)
	{
		m_preRenderState();
	}

	static float rot = 0;
	rot += .00025f;
	if (rot > 6.28f)
		rot = 0.0f;

	m_pos.x = 70 * cos(rot);
	m_pos.z = 70 * sin(rot);
	ImGui::Text("cube position:%f, %f", m_pos.x, m_pos.z);
	auto translation = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto ret = m_world * translation *   m_viewProj;
	D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &XMMatrixTranspose(ret), 0, 0);
	D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	D3D::deviceContext->IASetVertexBuffers(0, 1, &m_VBuffer, &m_stride, &m_offset);
	D3D::deviceContext->IASetIndexBuffer(m_IBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::deviceContext->DrawIndexed(m_indiceSize, 0, 0);
}

