#pragma once
#include "stdafx.h"
#include "IRender.h"
#include "Vertex.h"
#include "D3D.h"
#include <vector>
#include <functional>
class Cube : public IRender {

public:
	Cube();
	Cube(float size, XMFLOAT4 color);
	inline void SetPos(XMFLOAT3 pos) { m_pos = pos; }
	~Cube();

	virtual void PreRenderState(std::function<void() > preState) override;


	virtual void SetViewProj(const DirectX::XMMATRIX& ref) override;


	virtual void Update(float delta) const override;


	virtual void Draw() override;
protected:
	XMFLOAT3 m_pos;
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
	UINT m_indiceSize;

};