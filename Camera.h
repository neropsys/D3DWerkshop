#pragma once
#include <DirectXMath.h>
#include "D3D.h"
class Camera
{
public:


	Camera(int width, int height);
	void Update(double delta);
	inline const D3D::cbPerObject getConstantBufferObj() { return cbPerObj; };
	~Camera();

private:
	Camera();
	D3D::cbPerObject cbPerObj;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX wvp;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;
};
