Texture2D bloom : register(t0);

SamplerState sampAni;
Texture2D <uint2> stencilBuffer : register(t1);
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float4 sampleTex = bloom.Sample(sampAni, input.tex);
	float blurr = stencilBuffer[input.pos.xy * 4].g;
	//bool blurr = (xd > 0 ? 1 : 0);
	float3 fragmentColour = sampleTex.xyz * (blurr > 0 ? 1 : 0);
	
	return float4(fragmentColour, 1.0f);
};