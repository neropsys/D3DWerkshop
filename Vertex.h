#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{

	XMFLOAT3 pos;
	XMFLOAT4 color;
	Vertex(float x, float y, float z)
		: pos(x, y, z), color(1.f, 0.f, 0.f, 1.f)
	{}
	Vertex(XMFLOAT3 input, XMFLOAT4 in_color = XMFLOAT4(1, 0, 0, 1)):pos(input), color(in_color) {}
	Vertex(float x, float y, float z, float r, float g, float b = 0.f, float a = 0.f)
		: pos(x, y, z), color(r, g, b, a)
	{}
	void setUV(float x, float y) { color.x = x; color.y = y; color.w = 0.f; } //w는 텍스쳐인지 확인용
	bool Vertex::operator==(const Vertex& rh) const
	{
		if (pos.x == rh.pos.x &&
			pos.y == rh.pos.y &&
			pos.z == rh.pos.z &&
			color.x == rh.pos.x &&
			color.y == rh.color.y &&
			color.z == rh.color.z &&
			color.w == rh.color.w)
			return true;

		return false;

	}
};

struct StandardVertex
{
	StandardVertex(
		float x,
		float y,
		float z, 
		float u,
		float v, 
		float normx, 
		float normy,
		float normz):
	pos(x, y, z), tex(u, v), norm(normx, normy, normz){}
	StandardVertex(float x, float y, float z):pos(x, y, z), tex(0, 0), norm(0, 0, 0){}
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 tex;

};