Texture2D elementTex : register(t0);

SamplerState sampAni;

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 colour;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float alpha : ALPHA;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float4 elementColour = elementTex.Sample(sampAni, input.tex).xyzw;
	
	if (colour.x != 0.0f || colour.y != 0.0f || colour.z != 0.0f)
		elementColour.xyz = colour.xyz;
	
	elementColour.w *= input.alpha;
	return elementColour;
};