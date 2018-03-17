#include "ConstantBuffer.hlsl"
#include "Vertex.hlsli"
VS_OUTPUT_TEX main( float4 pos : POSITION, float4 normal : NORMAL, float2 inTexCoord : TEXCOORD)
{
	VS_OUTPUT_TEX output;
	output.Pos = mul(pos, WVP);
	output.Normal = normal;
	output.tex = inTexCoord;

	return output;
}