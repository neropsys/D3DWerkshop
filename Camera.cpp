#include "stdafx.h"
#include "Camera.h"
#include <math.h>

Camera::Camera(int width, int height) :
	camPos(DirectX::XMVectorSet(0.f, 0.f, -2.f, 0.0f)),
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
	world = DirectX::XMMatrixRotationRollPitchYaw(0, timeDelta, 0);

	wvp = world * view * proj;
	cbPerObj.WVP = DirectX::XMMatrixTranspose(wvp);
}
