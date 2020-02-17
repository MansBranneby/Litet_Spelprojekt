
struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORDS;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float alpha : ALPHA;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1), VP);
	output.tex = input.tex.xy;
	output.alpha = input.normal.x;

	return output;
}