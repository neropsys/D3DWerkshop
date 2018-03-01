#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

namespace D3D {
	struct cbPerObject
	{
		DirectX::XMMATRIX WVP;
	};
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	extern ID3D11Device* device;
	extern IDXGISwapChain* swapChain;
	extern ID3D11RenderTargetView* renderTargetView;
	extern ID3D11DeviceContext* deviceContext;
	extern ID3D11Texture2D* depthStencilBuffer;
	extern ID3D11DepthStencilState* depthStencilState;
	extern ID3D11DepthStencilView* depthStencilView;
	extern ID3D11RasterizerState* normalState;

	const float SCREEN_DEPTH = 100.0f;
	const float SCREEN_NEAR = 1.f;
	bool InitD3D(HINSTANCE hInst,
		int width, int height,
		HWND hwnd);
	void BeginScene();
	void EndScene();
	void Cleanup();
}