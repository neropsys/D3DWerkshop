struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT_TEX
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD0;
	float3 tan : TANGENT;
	float4 worldPos : POSITION0;
	float3 bitan : TEXCOORD1;
	float pad : PSIZE0;
};