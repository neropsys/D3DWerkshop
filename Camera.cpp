#include "stdafx.h"
#include "Camera.h"
#include <math.h>
#include "DInput.h"
Camera::Camera(int width, int height) :
	camPos(DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.0f)),
	camTarget(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f)),
	camUp(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f)),
	
	proj(DirectX::XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f)),
	world(DirectX::XMMatrixIdentity())
{
	view = DirectX::XMMatrixLookAtLH(camPos, camTarget, camUp);
}

Camera::~Camera()
{
}
void Camera::Update(double delta)
{
	static double timeDelta = (double)timeGetTime();
	timeDelta += (double)timeGetTime()* 0.0007f;
	timeDelta = sin(timeDelta);
	world = DirectX::XMMatrixRotationRollPitchYaw(0, timeDelta, 0);// ¸ðµ¨¿¡¼­µµ ÇØÁà¾ßÇÔ.
	auto identityMatrix = DirectX::XMMatrixIdentity();
	static float x = 0.f;
	static float y = 0.f;
	static float z = 0.f;
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
	world *= DirectX::XMMatrixTranslation(x, y, z);


	wvp = world * view * proj;
	cbPerObj.WVP = DirectX::XMMatrixTranspose(wvp);
}
