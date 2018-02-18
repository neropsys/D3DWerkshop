#include "stdafx.h"
#include "DInput.h"

LPDIRECTINPUT8 DInput::mDirectInput = nullptr;
IDirectInputDevice8* DInput::mpKeyboard = nullptr;
IDirectInputDevice8* DInput::mpMouse = nullptr;
std::array<BYTE, 256> DInput::mKeyboardState;
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
	ZeroMemory(mKeyboardState.data(), 256);
}

void DInput::Update(double time)
{
	DIMOUSESTATE mouseState;
	

	mpKeyboard->Acquire();
	mpMouse->Acquire();

	mpMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	mpKeyboard->GetDeviceState(sizeof(mKeyboardState), (LPVOID)&mKeyboardState);

}
