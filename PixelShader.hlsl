#include "Vertex.hlsl"
float4 main(VS_OUTPUT input) : SV_TARGET
{
	if(input.Color.w != 0)
		return input.Color;
	else return ObjTexture.Sample(ObjSamplerState, input.Color.xy);
}