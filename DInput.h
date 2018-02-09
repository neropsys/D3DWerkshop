#pragma once
#include <dinput.h>
#include <array>
class DInput
{
public:
	static HRESULT Init(HINSTANCE hInstance, HWND hwnd);
	static void Release();
	static void Reset();
	static void Update(double time);
	inline static const bool GetKeyboardState(int keycode) { 
		if (keycode >= 256 || keycode < 0) return false;
		return mKeyboardState[keycode] & 0x80;
	};
	
protected:
	static std::array<BYTE, 256> mKeyboardState;
	static IDirectInputDevice8* mpKeyboard;
	static IDirectInputDevice8* mpMouse;
	DIMOUSESTATE mLastMouseState;
	static LPDIRECTINPUT8 mDirectInput;

};