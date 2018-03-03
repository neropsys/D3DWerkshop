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
	static void Input(float time);
	inline static const bool GetKeyboardState(int keycode) { 
		if (keycode >= 256 || keycode < 0) return false;
		return m_keyboardState[keycode] & 0x80;
	};
	inline static const float MouseDeltaX()
	{
		return m_mouseXDelta;
	}
	inline static const float MouseDeltaY()
	{
		return m_mouseYDelta;
	}
	
protected:
	static float m_mouseXDelta;
	static float m_mouseYDelta;
	static std::array<BYTE, 256> m_keyboardState;
	static IDirectInputDevice8* mpKeyboard;
	static IDirectInputDevice8* mpMouse;
	DIMOUSESTATE mLastMouseState;
	static LPDIRECTINPUT8 mDirectInput;

};