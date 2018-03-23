cbuffer cbPerObject : register(b0)
{
	float4x4 WVP;
	float4x4 worldMat;
	float3 camPos;
	float specValue;
};
cbuffer cbLights : register(b1)
{
	float3 lightPos;
	float3 lightColor;
}
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D metallicTex : register(t2);
Texture2D roughnessTex : register(t3);
Texture2D aoTex : register(t4);
SamplerState ObjSamplerState;
