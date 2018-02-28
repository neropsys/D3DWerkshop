#include "stdafx.h"
#include "Camera.h"
#include <math.h>
#include "DInput.h"
Camera::Camera(int width, int height) :
	m_pos(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.0f)),
	m_target(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f)),
	m_up(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f)),
	
	m_proj(DirectX::XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f))
{
	m_view = DirectX::XMMatrixLookAtLH(m_pos, m_target, m_up);
}

Camera::~Camera()
{
}
void Camera::Update(double delta)
{
	static double timeDelta = (double)timeGetTime();
	timeDelta += (double)timeGetTime()* 0.0007f;
	timeDelta = sin(timeDelta);

	auto identityMatrix = DirectX::XMMatrixIdentity();
	static float x = 0.f;
	static float y = 0.f;
	static float z = 1.f;
	float mult = .001f;
	if (DInput::GetKeyboardState(DIK_W))
	{
		z -= mult;
	}
	if (DInput::GetKeyboardState(DIK_S))
	{
		z += mult;
	}
	if (DInput::GetKeyboardState(DIK_A))
	{
		x += mult;
	}
	if (DInput::GetKeyboardState(DIK_D))
	{
		x -= mult;
	}
	if (DInput::GetKeyboardState(DIK_Q))
	{
		y -= mult;
	}
	if (DInput::GetKeyboardState(DIK_E))
	{
		y += mult;
	}

	static float right = .0f;
	if (DInput::GetKeyboardState(DIK_K))
	{
		right += mult;
	}
	if (DInput::GetKeyboardState(DIK_J))
	{
		right -= mult;
	}
	m_pos = DirectX::XMVectorSet(x, y, z, 0.f);
	
	m_target = DirectX::XMVectorSet(x, y, z-1, .0f);
// 	m_target = DirectX::XMVectorAdd(m_target, 
// 		DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(right))));
	m_view = DirectX::XMMatrixLookAtLH(m_pos, m_target, m_up);

}
