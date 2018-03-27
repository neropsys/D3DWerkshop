#include "ConstantBuffer.hlsl"
#include "Vertex.hlsli"

const float gamma = 2.2;

float3 fresnelSchlick(float3 V, float3 H, float3 F0)
{
	float VdotH = max(dot(H, V), 0);
	return F0 + (1 - F0) * pow((1 - VdotH), 5);
}
float DistributionGGX(float3 N, float3 H, float roughness)
{
	float PI = 3.1415926535897932384626433832795f;
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float3 NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	return ggx2 * ggx1;
}

float4 main(VS_OUTPUT_TEX input) : SV_TARGET
{
	float PI = 3.1415926535897932384626433832795f;
	float3 unitVector = { 1.0, 1.0, 1.0 };
	float3 albedo = albedoTex.Sample(ObjSamplerState, input.tex).rgb;
	float roughness = roughnessTex.Sample(ObjSamplerState, input.tex).r;
	float metallic = metallicTex.Sample(ObjSamplerState, input.tex).r;
	float AO = aoTex.Sample(ObjSamplerState, input.tex).r;
	float Opacity = opacityTex.Sample(ObjSamplerState, input.tex).r;

	float3 normal = normalTex.Sample(ObjSamplerState, input.tex).rgb;
	normal = normalize(normal * 2 - 1);
	float4 camWorldPos = mul(worldMat, camPos);

	float3 F0 = 0.04;
	F0 = lerp(F0, albedo, metallic);

	float3 T = normalize(input.tan);
	float3 B = normalize(input.bitan);
    float3 N = normalize(input.Normal);


    float3x3 TBN = transpose(float3x3(T, B, N));

    N = normalize(mul(TBN, normal));
 
	float3 V = normalize(camWorldPos - input.worldPos);

	float3 p = input.worldPos;

	float VdotN = max(dot(V, N), 0);
	float3 Lo = { 0, 0, 0 };
	
	{


		float distance = length(p - lightPos);
		float attenuation = 1.0f / (distance * distance);
		float3 Li = attenuation;


		float3 L = normalize(lightPos - p);
		float3 H = normalize(L + V);

		float3 fLambert = albedo / PI;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		float3 F = fresnelSchlick(V, H, F0);

		float3 kS = F;
		float3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;

		float3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		float3 specular = numerator / max(denominator, 0.001);

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * fLambert + specular) * Li * NdotL;
        
	}



	float3 directLight = Lo;

	float3 indirectLight =   albedo * AO;
	float3 color = indirectLight + Lo * specValue;

	return float4(color, Opacity);
}