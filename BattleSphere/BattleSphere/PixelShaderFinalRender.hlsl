Texture2D scene : register(t0);
Texture2D bloom : register(t1);

SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 sceneColour = scene.Sample(sampAni, input.tex).xyz;
	float3 bloomColour = bloom.Sample(sampAni, input.tex).xyz;

	float3 fragmentColour = sceneColour + bloomColour;

	return float4(fragmentColour, 1.0f);
};