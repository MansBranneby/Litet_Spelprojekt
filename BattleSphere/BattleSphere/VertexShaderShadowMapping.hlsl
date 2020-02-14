struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORDS;
	float3 normal : NORMAL;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
};

cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix M;
};

float4 VS_main(VS_IN input) : SV_POSITION
{
	return mul(VP, mul(M, float4(input.pos, 1.0f)));
}