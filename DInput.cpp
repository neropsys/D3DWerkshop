#include "stdafx.h"
#include "DInput.h"

LPDIRECTINPUT8 DInput::mDirectInput = nullptr;
IDirectInputDevice8* DInput::mpKeyboard = nullptr;
IDirectInputDevice8* DInput::mpMouse = nullptr;
std::array<BYTE, 256> DInput::m_keyboardState;
float DInput::m_mouseYDelta = 0;
float DInput::m_mouseXDelta = 0;
HRESULT DInput::Init(HINSTANCE hInstance, HWND hwnd)
{
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, NULL);

	if (!SUCCEEDED(hr))
		return hr;
	
	hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mpKeyboard, NULL);
	if (!SUCCEEDED(hr))
		return hr;
	
	hr = mDirectInput->CreateDevice(GUID_SysMouse, &mpMouse, NULL);
	if (!SUCCEEDED(hr))
		return hr;

	hr = mpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = mpKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = mpMouse->SetDataFormat(&c_dfDIMouse);
	hr = mpMouse->SetCooperativeLevel(hwnd,  DISCL_NOWINKEY | DISCL_FOREGROUND);

	return S_OK;

}

void DInput::Release()
{
	mpKeyboard->Unacquire();
	mpMouse->Unacquire();
	mDirectInput->Release();
}

void DInput::Reset()
{
	ZeroMemory(m_keyboardState.data(), 256);
}

void DInput::Update(double time)
{
	
	DIMOUSESTATE mouseState;


	mpKeyboard->Acquire();
	mpMouse->Acquire();

	mpMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	static DIMOUSESTATE lastState = mouseState;
	mpKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	
	m_mouseXDelta += (float)mouseState.lX-lastState.lX;
	m_mouseYDelta += (float)mouseState.lY -lastState.lY;
	lastState = mouseState;
}
