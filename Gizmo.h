#pragma once
#include "IRender.h"
#include "Vertex.h"
#include <vector>
#include <functional>
class Gizmo : public IRender {

public:
	Gizmo();
	~Gizmo();
	virtual void Update(float delta) const override;


	virtual void Draw() override;

	virtual void SetViewProj(const DirectX::XMMATRIX& ref) override;


	virtual void PreRenderState(std::function<void() > preState) override;

protected:
	std::function<void()> m_preRenderState;
	ID3D11Buffer * m_VBuffer;
	ID3D11Buffer* m_IBuffer;
	ID3D11Buffer* m_constantBuffer;
	UINT m_stride;
	UINT m_offset;
	std::vector<Vertex> m_vertice;
	std::vector<UINT> m_indice;
	XMMATRIX m_world;
	XMMATRIX m_viewProj;
	float m_magnitude = 10.f;
	float offset = 2.5f;
	UINT m_indiceSize;

	XMMATRIX m_xAxisWvp;
	XMMATRIX m_yAxisWvp;
	XMMATRIX m_zAxisWvp;
};