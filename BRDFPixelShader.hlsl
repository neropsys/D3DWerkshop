#include "ConstantBuffer.hlsl"
#include "Vertex.hlsli"
float4 main(VS_OUTPUT_TEX input) : SV_TARGET
{
	return ObjTexture.Sample(ObjSamplerState, input.tex);
}