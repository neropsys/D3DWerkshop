#pragma once
#include "IRender.h"
#include <vector>
#include "Vertex.h"
class Gizmo : public IRender {

public:
	Gizmo();
	~Gizmo();
	virtual void Update(float delta) const override;


	virtual void Draw() override;

	virtual void SetViewProj(const DirectX::XMMATRIX& ref) override;

protected:
	ID3D11Buffer * mVBuffer;
	ID3D11Buffer* mIBuffer;
	ID3D11Buffer* m_constantBuffer;
	UINT m_stride;
	UINT m_offset;
	UINT mindexCount;
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