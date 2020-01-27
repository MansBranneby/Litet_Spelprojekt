struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 col : COLOUR;
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

float4 PS_main(PS_IN input) : SV_Target
{
	////LIGHTING//

	//float3 ambientCol = { 0.2, 0.2, 0.2 };
	//float3 fragmentCol;

	////Illumination models
	//int illumModel = 2;

	//switch (illumModel)
	//{
	//case 0: // Constant colour (diffuse)
	//case 1: // Lambertian shading (diffuse, ambient)
	//case 2: // "Phong" (diffuse, ambient, specular)
	//	//Ambient
	//	float3 ambient = colour * ambientCol;

	//	if (pixelDepth < shadowMapDepth || !inLightFrustum)
	//	{
	//		//Diffuse
	//		float diffuseFactor = max(dot(normalize(lightPos - position), normalize(normal)), 0);
	//		float3 diffuse = colour * lightCol * diffuseFactor;

	//		//Specular
	//		float3 n = normalize(normal);
	//		float3 l = normalize(lightPos - position);
	//		float3 v = normalize(cameraPos - position);
	//		float3 r = normalize(2 * dot(n, l) * n - l);

	//		float3 specular = colour * lightCol * pow(max(dot(r, v), 0), 20);

	//		//Final
	//		fragmentCol = ambient + diffuse + specular;
	//	}
	//	else
	//		fragmentCol = ambient;
	//default:
	//};

	//return float4(fragmentCol, 1.0f);
	return float4(input.col, 1.0f);
};