#define BLOCK_SIZE 32
SamplerState sampAni;

struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 posWC : POSITIONWORLD;
	float2 tex : TEXCOORDS;
	float3 nor : NORMAL;
};
struct Light {
	int Enabled;
	int Type;
	float Range, SpotlightAngle;
	float4 Position;
	float3 Direction;
	float Intensity;
	float4 color;
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

float DoAttenuation(Light light, float d)
{
	return 1.0f - smoothstep(light.Range * 0.2f, light.Range, d);
}
float DoSpotCone(Light light, float4 L)
{
	
	float minCos = cos(radians(light.SpotlightAngle));
	float maxCos = lerp(minCos, 1, 0.5f);
	float cosAngle = dot(normalize(light.Direction), -L.xyz);
	return smoothstep(minCos, maxCos, cosAngle);
}

Texture2D<uint2> LightGrid : register(t0);
StructuredBuffer<uint> LightIndex : register(t1);
StructuredBuffer<Light> Lights : register(t2);
float4 PS_main(PS_IN input) : SV_Target
{
	////LIGHTING//// (for one light)
	uint2 tileIndex = uint2(floor(input.pos.xy / BLOCK_SIZE));
	uint startOffset = LightGrid[tileIndex].x;
	uint lightCount = LightGrid[tileIndex].y;

	
	float3 Ia = { 0.2, 0.2, 0.2 }; // Ambient light
	float3 fragmentCol;
	
	float3 Ka = float3(KaIn.x, KaIn.y, KaIn.z); // Ambient surface colour
	float3 Kd = float3(KdIn.x, KdIn.y, KdIn.z); // Diffuse surface colour
	float3 Ks = float3(KsIn.x, KsIn.y, KsIn.z); // Specular surface colour
	float3 Ke = float3(KeIn.x, KeIn.y, KeIn.z); // Emissive surface colour
	float Ns = KsIn.w; // Specular shininess
	float3 normal = normalize(input.nor); // Surface normal
	float3 V = normalize(float3(cameraPos.x, cameraPos.y, cameraPos.z) - input.posWC); // Vector towards camera
	fragmentCol = Ka * Ia;
	for (unsigned int i = startOffset; i < startOffset + lightCount; i++)
	{
		Light light = Lights[LightIndex[i]];
		float4 lightPos = light.Position;
		
		// Calculate for every lightsource
		float d = pow(length(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC), 1); // Attenuation (decay of light, increase the power to to increase effect)
		float3 L = float3(0,0,0);
		float3 R = float3(0,0,0);
		float4 lightCol = float4(0,0,0,0);
		switch (light.Type) 
		{
		case 0:
			//Point light
			L = normalize(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC); // Vector towards light
			R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface
			lightCol = Lights[LightIndex[i]].color * DoAttenuation(light, d) * light.Intensity;
			break;
		case 1:
			//Directional light

			L = normalize(-Lights[LightIndex[i]].Direction.xyz);
			R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface
			lightCol = Lights[LightIndex[i]].color * light.Intensity;
			break;
		case 2:
			//return float4(1, 1, 1, 1);
			L = normalize(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC); // Vector towards light
			R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface
			float attenuation = DoAttenuation(light, d);
			float spotIntensity = DoSpotCone(light, float4(L, 1.0f));
			lightCol = Lights[LightIndex[i]].color * attenuation * spotIntensity * light.Intensity;
			break;
		}
		
		


		// Illumination models //
		switch (KaIn.w)
		{
		case 0: // Constant colour (diffuse)
			fragmentCol = Kd;
			break;
		case 1: // Lambertian shading (diffuse, ambient)
			fragmentCol += Kd * max(dot(normal, L), 0.0f) * (float3)lightCol;
			break;
		case 2: // "Phong" (diffuse, ambient, specular)
			fragmentCol += (Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol) + Ke;

			break;
		case 3: // "Phong" (diffuse, ambient, specular) + ray tracing (not implemented)
			fragmentCol += (Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + Ks * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol);
			break;
		default:
			break;
		};

	}
	
	if (Ke.x > 0 || Ke.y > 0 || Ke.z > 0)
		return float4(fragmentCol, 1.0f);
	else
		return float4(fragmentCol, 0.0f);
};