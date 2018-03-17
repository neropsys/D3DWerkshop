struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT_TEX
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
};