#include "stdafx.h"
#include "Camera.h"
#include <math.h>
#include "DInput.h"
#include "imgui.h"
Camera::Camera(int width, int height) :
	m_pos(DirectX::XMVectorSet(0.f, 0.f, -10.f, 0.0f)),
	m_target(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f)),
	m_up(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f)),
	m_forward(DirectX::XMVectorSet(0, 0, 1.f, 0)),
	m_right(DirectX::XMVectorSet(1, 0, 0, 0)),
	m_proj(DirectX::XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f))
{
	m_view = DirectX::XMMatrixLookAtLH(m_pos, m_target, m_up);
}

Camera::~Camera()
{
}
void Camera::Update(double delta)
{
	using namespace DirectX;
	auto identityMatrix = XMMatrixIdentity();
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float mult = .01f;



	if (DInput::GetKeyboardState(DIK_W))
	{
		z += mult;
	}
	if (DInput::GetKeyboardState(DIK_S))
	{
		z -= mult;
	}
	if (DInput::GetKeyboardState(DIK_A))
	{
		x -= mult;
	}
	if (DInput::GetKeyboardState(DIK_D))
	{
		x += mult;
	}
	if (DInput::GetKeyboardState(DIK_Q))
	{
		y += mult;
	}
	if (DInput::GetKeyboardState(DIK_E))
	{
		y -= mult;
	}

	static float right = .0f;
	if (DInput::GetKeyboardState(DIK_K))
	{
		right -= mult;
	}
	if (DInput::GetKeyboardState(DIK_J))
	{
		right += mult;
	}
	if (DInput::GetKeyboardState(DIK_R))
	{
		right = 0;
		m_pos = XMVectorSet(0.f, 0.f, -1.f, 0.0f);
		m_target = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		m_up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	}
	m_yaw += DInput::MouseDeltaX() * mult;
	m_pitch += DInput::MouseDeltaY() * mult;



	m_target = XMVectorSet(x, y, z-1, .0f);
	ImGui::Text("right:%f", right);
	ImGui::Text("delta x:%f, delta y:%f", DInput::MouseDeltaX(), DInput::MouseDeltaY());
	auto rotMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0);


	m_target = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), rotMatrix);
	m_target = XMVector3Normalize(m_target);
	auto yRot = XMMatrixRotationY(m_yaw);

	m_right = XMVector3TransformCoord(XMVectorSet(1, 0, 0, 0), yRot);
	m_up = XMVector3TransformCoord(m_up, yRot);
	m_forward = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), yRot);

	m_pos += x * m_right;
	m_pos += z * m_forward;
	m_pos += XMVectorSet(0, y, 0, 0);


	m_target = m_pos + m_target;
	
	m_view = XMMatrixLookAtLH(m_pos, m_target, m_up);

}
