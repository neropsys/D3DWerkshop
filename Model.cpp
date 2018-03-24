#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include "D3D.h"
#include "Utilities.h"
Model::Model(const char * fileName):Model()
{

	auto filePath = path::GetAbsPath(fileName);
	auto delimitPos = filePath.find_last_of("\\/");
	std::string subDir = "";
	if(delimitPos != std::string::npos)
	{
		subDir = filePath.substr(0, delimitPos);
	}

	assert(mloader.Load(subDir.c_str(), fileName));
	static_assert(sizeof(StandardVertex) % 16 == 0, "vertex must be 16byte aligned");

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(StandardVertex) *  mloader.GetVertexList().size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = &mloader.GetVertexList().data()[0];

	auto hr = D3D::device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_VBuffer);
	
	assert(SUCCEEDED(hr));
	{
		const char c_szName[] = "VERTEX BUFFER";
		m_VBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
			sizeof(c_szName) - 1, c_szName);
	}
	m_stride = sizeof(StandardVertex);
	m_offset = 0;

	m_indexCount = mloader.GetIndexCount();

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * mloader.GetIndexCount();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	int indexCount = mloader.GetIndexCount();
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = mloader.GetIndexList().data();

	hr = D3D::device->CreateBuffer(&indexBufferDesc, &initData, &m_IBuffer);
	assert(SUCCEEDED(hr));
	{
		const char c_szName[] = "index BUFFER";
		m_IBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
			sizeof(c_szName) - 1, c_szName);
	}
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	hr = D3D::device->CreateRasterizerState(&desc, &m_alphaBlend);

	{
		const char c_szName[] = "index m_wireframe";
		m_alphaBlend->SetPrivateData(WKPDID_D3DDebugObjectName,
			sizeof(c_szName) - 1, c_szName);
	}
	assert(SUCCEEDED(hr));
	//D3D::deviceContext->IASetVertexBuffers(0, 1, &mVBuffer, &stride, &offset);
	//D3D::deviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	static_assert(sizeof(D3D::cbPerObject) % 16 == 0, "vertex must be 16byte aligned");

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.ByteWidth = sizeof(D3D::cbPerObject);
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cbbd, NULL, &m_constantBuffer)));
	{
		const char c_szName[] = "constant buffer";
		m_constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
			sizeof(c_szName) - 1, c_szName);
	}

	static_assert(sizeof(D3D::cbLight) % 16 == 0, "vertex must be 16byte aligned");

	D3D11_BUFFER_DESC cblb;
	ZeroMemory(&cblb, sizeof(D3D11_BUFFER_DESC));

	cblb.Usage = D3D11_USAGE_DEFAULT;
	cblb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cblb.ByteWidth = sizeof(D3D::cbLight);
	cblb.CPUAccessFlags = 0;
	cblb.MiscFlags = 0;

	assert(SUCCEEDED(D3D::device->CreateBuffer(&cblb, NULL, &m_lightBuffer)));
	{
		const char c_szName[] = "light buffer";
		m_constantBuffer->SetPrivateData(WKPDID_D3DDebugObjectName,
			sizeof(c_szName) - 1, c_szName);
	}



	if (mloader.HasTexture() == false)
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
		hr = D3D::device->CreateSamplerState(&sampDesc, &m_samplerState);
		assert(SUCCEEDED(hr));
		//D3D::deviceContext->PSSetShaderResources()
// 		for (const auto& it : mloader.GetTextureList())
// 		{
// 		}
	}

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.RenderTarget[0] = rtbd;

	D3D::device->CreateBlendState(&blendDesc, &m_blendState);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = D3D::device->CreateRasterizerState(&cmdesc, &m_ccwRasterState);

	cmdesc.FrontCounterClockwise = false;
	hr = D3D::device->CreateRasterizerState(&cmdesc, &m_cwRasterState);
}

Model::Model() :
	m_VBuffer(nullptr),
	m_IBuffer(nullptr),
	m_stride(0),
	m_offset(0),
	m_indexCount(0),
	m_alphaBlend(nullptr),
	m_setwireframe(false),
	m_constantBuffer(nullptr),
	m_world(XMMatrixIdentity()),
	m_samplerState(nullptr),
	m_preState(nullptr),
	m_blendState(nullptr)
{
	mloader.Init();
}


Model::~Model()
{
	using namespace D3D;
	Release(m_VBuffer);
	Release(m_IBuffer);
	Release(m_constantBuffer);
	Release(m_alphaBlend);
	Release(m_samplerState);
	Release(m_lightBuffer);
	Release(m_blendState);
	Release(m_cwRasterState);
	Release(m_ccwRasterState);
}

void Model::Update(float delta) const
{
	auto ret = m_world * m_viewProj;
	D3D::cbPerObject cBuffer;
	cBuffer.WVP = XMMatrixTranspose(ret);
	cBuffer.world = m_world;
	cBuffer.camPos = m_camPos;
	static float specValue;
	ImGui::SliderFloat("shininess", &specValue, 1, 100000);
	cBuffer.specValue = specValue;
	static D3D::cbLight light;
	light.pos = { 35, 35, 35 };

	light.color = { 255, 255, 255 };
	static float rot = 0;
	rot += .00025f;
	if (rot > 6.28f)
		rot = 0.0f;
	light.pos.x = 70 * cos(rot);
	light.pos.z = 70 * sin(rot);
	ImGui::Text("light position:%f, %f", light.pos.x, light.pos.z);

	D3D::deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cBuffer, 0, 0);
	D3D::deviceContext->UpdateSubresource(m_lightBuffer, 0, NULL, &light, 0, 0);
}

void Model::Draw() 
{

	if (m_preState != nullptr)
	{
		m_preState();
	} 
	D3D::deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	D3D::deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffer);
	D3D::deviceContext->PSSetConstantBuffers(1, 1, &m_lightBuffer);
	D3D::deviceContext->IASetVertexBuffers(0, 1, &m_VBuffer, &m_stride, &m_offset);
	D3D::deviceContext->IASetIndexBuffer(m_IBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (true)
	{
		//D3D::deviceContext->RSSetState(m_alphaBlend);
	}

	D3D::deviceContext->OMSetBlendState(m_blendState, 0, 0xffffffff);
	D3D::deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	D3D::deviceContext->PSSetShaderResources(0, 1, mloader.GetTexture(ModelLoader::DIFFUSE));
	D3D::deviceContext->PSSetShaderResources(1, 1, mloader.GetTexture(ModelLoader::NORMAL));
	D3D::deviceContext->PSSetShaderResources(2, 1, mloader.GetTexture(ModelLoader::METALLIC));
	D3D::deviceContext->PSSetShaderResources(3, 1, mloader.GetTexture(ModelLoader::ROUGHNESS));
	D3D::deviceContext->PSSetShaderResources(4, 1, mloader.GetTexture(ModelLoader::AO));
	D3D::deviceContext->PSSetShaderResources(5, 1, mloader.GetTexture(ModelLoader::OPACITY));
	D3D::deviceContext->RSSetState(m_ccwRasterState);
	D3D::deviceContext->DrawIndexed(m_indexCount, 0, 0);
	D3D::deviceContext->RSSetState(m_cwRasterState);
	D3D::deviceContext->DrawIndexed(m_indexCount, 0, 0);
	if (m_setwireframe)
	{
		D3D::deviceContext->RSSetState(0);
	}


	//todo: reset state?
}

void Model::SetViewProj(const DirectX::XMMATRIX& ref)
{
	m_viewProj = ref;
}

void Model::PreRenderState(std::function<void() > preState)
{
	m_preState = preState;
}

