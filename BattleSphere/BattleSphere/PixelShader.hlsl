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

struct PS_OUT
{
	float4 scene : SV_Target0;
	float4 bloom : SV_Target1;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	////LIGHTING//// (for one light)

	float3 Ia = { 0.2, 0.2, 0.2 }; // Ambient light
	float3 fragmentCol;
	
	float3 Ka = float3(KaIn.x, KaIn.y, KaIn.z); // Ambient surface colour
	float3 Kd = float3(KdIn.x, KdIn.y, KdIn.z); // Diffuse surface colour
	float3 Ks = float3(KsIn.x, KsIn.y, KsIn.z); // Specular surface colour
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
		fragmentCol = Ka * Ia + Kd * max(dot(normal, L), 0.0f) * (float3)lightCol * d;
		break;
	case 2: // "Phong" (diffuse, ambient, specular)
		fragmentCol = Ka * Ia + input.col * max(dot(normal, L), 0.0f) * (float3)lightCol * d + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol * d;
		break;
	case 3: // "Phong" (diffuse, ambient, specular) + ray tracing (not implemented)
		fragmentCol = Ka * Ia + Kd * max(dot(normal, L), 0.0f) * (float3)lightCol * d + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol * d;
		break;
	default:
		fragmentCol = float3(1.0f, 1.0f, 1.0f);
		break;
	};

	output.scene = float4(fragmentCol, 1.0f);
	output.bloom = float4(fragmentCol, 1.0f);

	return output;
};