#include "Vertex.hlsl"
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return input.Color;
}