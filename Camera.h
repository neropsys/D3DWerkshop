#pragma once
#include <DirectXMath.h>
#include "D3D.h"
class Camera
{
public:


	Camera(int width, int height);
	void Update(double delta);
	inline const DirectX::XMMATRIX getViewProj() { return m_view * m_proj; };
	~Camera();

private:
	Camera();
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_proj;


	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_target;
	DirectX::XMVECTOR m_up;
};
