Texture2D tex : register(t0);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 texColour = tex.Sample(sampAni, input.tex).xyz;

	return float4(1.0f, 0.0f, 0.0f, 1.0f);
};