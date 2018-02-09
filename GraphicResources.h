#pragma once
#include "D3D.h"
#include "Camera.h"
namespace Resources {
	void InitResources();
	void UpdateResources(const D3D::cbPerObject& wvp);
	void DrawResources();
	extern ID3D11Buffer* g_indiceBuffer;
	extern ID3D11Buffer* g_triIndiceBuffer;
	extern ID3D11Buffer* g_squareVertexBuffer;
	extern ID3D11Buffer* g_triVertexBuffer;
	extern ID3D11VertexShader* g_vertexShader;
	extern ID3D11PixelShader* g_pixelShader;
	extern ID3D10Blob* g_vsBuffer;
	extern ID3D10Blob* g_psBuffer;
	extern ID3D11InputLayout* g_inputLayout;

	void Release();
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	
}