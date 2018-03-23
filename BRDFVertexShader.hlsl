#include "ConstantBuffer.hlsl"
#include "Vertex.hlsli"
VS_OUTPUT_TEX main( float4 pos : POSITION, float3 normal : NORMAL, float2 inTexCoord : TEXCOORD, float3 inTan : TANGENT)
{
	VS_OUTPUT_TEX output;
	output.Pos = mul(pos, WVP);
	output.Normal = normal.xyz;
	output.tex = inTexCoord;
	output.tan = inTan;
	output.worldPos = pos; // mul(pos, worldMat);
	output.bitan = cross(normal, inTan);

	return output;
}