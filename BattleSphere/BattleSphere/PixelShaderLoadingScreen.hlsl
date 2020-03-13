Texture2D loadingScreen : register(t0);

SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 fragmentColour = loadingScreen.Sample(sampAni, input.tex).xyz;

	return float4(fragmentColour, 1.0f);
};