#pragma once
#include <DirectXMath.h>
using namespace DirectX;
struct Vertex
{
	Vertex(float x, float y, float z)
		: pos(x, y, z), color(1.f, 0.f, 0.f, 1.f), cpIndex(0)
	{}
	XMFLOAT3 pos;

	XMFLOAT4 color;
	UINT cpIndex;
	Vertex(XMFLOAT3 input, XMFLOAT4 in_color = XMFLOAT4(1, 0, 0, 1)) :pos(input), color(in_color), cpIndex(0) {}
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: pos(x, y, z), color(r, g, b, a), cpIndex(0)
	{}
	bool Vertex::operator==(const Vertex& rh) const
	{
		if (pos.x == rh.pos.x &&
			pos.y == rh.pos.y &&
			pos.z == rh.pos.z &&
			color.x == rh.pos.x &&
			color.y == rh.color.y &&
			color.z == rh.color.z &&
			color.w == rh.color.w &&
			cpIndex == rh.cpIndex)
			return true;

		return false;

	}
};
