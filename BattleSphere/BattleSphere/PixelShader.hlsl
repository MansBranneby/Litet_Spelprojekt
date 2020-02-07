SamplerState sampAni;

struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 posWC : POSITIONWORLD;
	float2 tex : TEXCOORDS;
	float3 nor : NORMAL;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 lightPos;
	float4 lightCol;
};

cbuffer PS_CONSTANT_BUFFER : register(b1)
{
	float4 cameraPos;
};

cbuffer PS_CONSTANT_BUFFER : register(b2)
{
	float4 KaIn; //Ka + illum
	float4 KdIn; //Kd + Ni
	float4 KsIn; //Ks + Ns
	float4 KeIn; //Ke + d
};

float4 PS_main(PS_IN input) : SV_Target
{
	////LIGHTING//// (for one light)

	float3 Ia = { 0.2, 0.2, 0.2 }; // Ambient light
	float3 fragmentCol;
	
	float3 Ka = float3(KaIn.x, KaIn.y, KaIn.z); // Ambient surface colour
	float3 Kd = float3(KdIn.x, KdIn.y, KdIn.z); // Diffuse surface colour
	float3 Ks = float3(KsIn.x, KsIn.y, KsIn.z); // Specular surface colour
	float3 Ke = float3(KeIn.x, KeIn.y, KeIn.z); // Emissive surface colour
	float Ns = KsIn.w; // Specular shininess
	float3 normal = normalize(input.nor); // Surface normal
	float3 V = normalize(float3(cameraPos.x, cameraPos.y, cameraPos.z) - input.posWC); // Vector towards camera
	
	// Calculate for every lightsource
	float3 d = 1 / pow(length(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC), 1); // Attenuation (decay of light, increase the power to to increase effect)
	float3 L = normalize(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC); // Vector towards light
	float3 R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface

	// Illumination models //
	switch (KaIn.w)
	{
	case 0: // Constant colour (diffuse)
		fragmentCol = Kd;
		break;
	case 1: // Lambertian shading (diffuse, ambient)
		fragmentCol = Kd * Ia + Kd * max(dot(normal, L), 0.0f) * (float3)lightCol;
		//fragmentCol = float3(0.0f, 1.0f, 0.0f);
		break;
	case 2: // "Phong" (diffuse, ambient, specular)
		fragmentCol = Kd * Ia + Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol + Ke;
		break;
	case 3: // "Phong" (diffuse, ambient, specular) + ray tracing (not implemented)
		fragmentCol = Kd * Ia + Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol;
		break;
	default:
		fragmentCol = float3(1.0f, 1.0f, 1.0f);
		break;
	};

	if(Ke.x > 0 || Ke.y > 0 || Ke.z > 0)
		return float4(fragmentCol, 1.0f);
	else
		return float4(fragmentCol, 0.0f);
};