Texture2D elementTex : register(t0);

SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float alpha : ALPHA;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float4 elementColour = elementTex.Sample(sampAni, input.tex).xyzw;
	elementColour.w *= input.alpha;

	return elementColour;
};