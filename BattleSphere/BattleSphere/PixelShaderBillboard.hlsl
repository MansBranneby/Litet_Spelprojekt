SamplerState sampAni : register(s0);

#define BLOCK_SIZE 32

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

cbuffer PS_CONSTANT_BUFFER : register(b3)
{
	matrix lightVP;
};

cbuffer PS_ROBOT_DATA : register (b4)
{
	float4 playerPosition[4];
};

cbuffer PS_CONSTANT_BUFFER : register (b5)
{
	float4 velocityUV;
	float blinkValue;
	float billboardType;
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
Texture2D txShadowMap : register(t3);
Texture2D txModel : register(t4); // Model texture

float4 PS_main(PS_IN input) : SV_Target
{
	////LIGHTING//// (for one light)
	uint2 tileIndex = uint2(floor(input.pos.xy / BLOCK_SIZE));
	uint startOffset = LightGrid[tileIndex].x;
	uint lightCount = LightGrid[tileIndex].y;

	// Shadow
	float4 PosRelLight = mul(lightVP, float4(input.posWC.xyz, 1.0f));

	PosRelLight.xy /= PosRelLight.w;

	float2 shadowMapTex = float2(0.5f * PosRelLight.x + 0.5f, -0.5f * PosRelLight.y + 0.5f);
	float depth = PosRelLight.z / PosRelLight.w;

	float ep = 0.0005f;
	float dx = 1.0f / 1920;
	float dy = 1.0f / 1080;

	float sum = 0;
	float x, y;
	for (y = -1.0; y <= 1.0; y += 0.5)
		for (x = -1.0; x <= 1.0; x += 0.5)
			sum += (txShadowMap.Sample(sampAni, shadowMapTex + float2(dx * x, dy * y)).r + ep < depth) ? 0.0f : 1.0f;
	float shadowCoeff = sum / 25.0;

	float3 Ia = { 0.5, 0.5, 0.5 }; // Ambient light
	float3 fragmentCol;

	float3 Ka = float3(KaIn.x, KaIn.y, KaIn.z); // Ambient surface colour
	float3 Kd = float3(KdIn.x, KdIn.y, KdIn.z); // Diffuse surface colour
	float3 Ks = float3(KsIn.x, KsIn.y, KsIn.z); // Specular surface colour
	float3 Ke = float3(KeIn.x, KeIn.y, KeIn.z); // Emissive surface colour

	float Ns = KsIn.w; // Specular shininess
	float3 normal = normalize(input.nor); // Surface normal
	float3 V = normalize(float3(cameraPos.x, cameraPos.y, cameraPos.z) - input.posWC); // Vector towards camera
	fragmentCol = Ia * Kd; // * Ia
	for (unsigned int i = startOffset; i < startOffset + lightCount; i++)
	{

		Light light = Lights[LightIndex[i]];
		float4 lightPos = light.Position;
		float3 LKs = Ks;
		//
		// Calculate for every lightsource
		float d = pow(length(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC), 1); // Attenuation (decay of light, increase the power to to increase effect)
		float3 L = float3(0, 0, 0);
		float3 R = float3(0, 0, 0);

		float4 lightCol = float4(0, 0, 0, 0);
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
	lightCol = Lights[LightIndex[i]].color * light.Intensity * shadowCoeff;
	break;
case 2:
	//return float4(1, 1, 1, 1);
	L = normalize(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC); // Vector towards light
	R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface
	float attenuation = DoAttenuation(light, d);
	float spotIntensity = DoSpotCone(light, float4(L, 1.0f));
	lightCol = Lights[LightIndex[i]].color * attenuation * spotIntensity * light.Intensity;

	break;
case 3:
	//Area light

	L = normalize(float3(lightPos.x, lightPos.y, lightPos.z) - input.posWC); // Vector towards light
	R = normalize(2 * dot(normal, L) * normal - L); // Reflection of light on surface
	lightCol = Lights[LightIndex[i]].color * DoAttenuation(light, d) * light.Intensity;

	LKs = float3(0, 0, 0);

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
			fragmentCol += (Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + LKs * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol);

			break;
		case 3: // "Phong" (diffuse, ambient, specular) + ray tracing (not implemented)
			fragmentCol += Kd * max(dot(normal, L), 0.0f) * (float3)lightCol + LKs * pow(max(dot(R, V), 0.0f), Ns) * (float3)lightCol;
			break;
		default:
			fragmentCol = float3(1.0f, 1.0f, 1.0f);
			break;
		};
	}

	if (Ke.x > 0 || Ke.y > 0 || Ke.z > 0)
		fragmentCol = Ke;
	float3 ndcSpace = float3(0,0,0);
	ndcSpace.x = input.pos.x / 1920; // 0 - 1
	ndcSpace.x = ndcSpace.x * 2 - 1;
	ndcSpace.y = -input.pos.y / 1080; // 0 - 1

	ndcSpace.y = (ndcSpace.y * 2 + 1);
	ndcSpace.z = input.pos.z;
	float aspectRatio = 9.0f / 16.0f;
	for (int j = 0; j < 4; j++)
	{
		float2 dist = playerPosition[j].xy - ndcSpace.xy;

		dist.y *= aspectRatio;
		if (length(dist) < playerPosition[j].w * 4 && ndcSpace.z < playerPosition[j].z && input.posWC.y > 0.2f)
			return float4(fragmentCol, 0.3f);
	}

	// Sample texture with UV coordinates that have been incremented with velocityUV coordinates to translate the texture
	if (billboardType == 1.0f)
	{
		float3 modelTexture = txModel.Sample(sampAni, input.tex + velocityUV.xy).xyz;

		if (Ke.x > 0.0f || Ke.y > 0.0f || Ke.y > 0.0f)
		{
			modelTexture *= blinkValue;
			fragmentCol *= blinkValue;
		}

		if (input.posWC.y > 20.0f && input.posWC.y < 55.0f)
			fragmentCol = lerp(fragmentCol, float3(1.0f, 1.0f, 1.0f), -(20.0f - input.posWC.y) / 35.0f);

		/*if (input.posWC.y > 45.0f && input.posWC.y < 55.0f)
			fragmentCol = lerp(fragmentCol, float3(1.0f, 1.0f, 1.0f), (55.0f - input.posWC.y)/20);
		else if (input.posWC.y > 35.0f && input.posWC.y < 45.0f)
			fragmentCol = lerp(fragmentCol, float3(1.0f, 1.0f, 1.0f), -(33.0f - input.posWC.y) / 20);*/



		return float4(modelTexture + fragmentCol, KeIn.w);
	}



	return float4(fragmentCol , KeIn.w);
};