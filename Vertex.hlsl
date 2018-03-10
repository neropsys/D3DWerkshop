struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};
cbuffer cbPerObject
{
	float4x4 WVP;
};
Texture2D ObjTexture;
SamplerState ObjSamplerState;
