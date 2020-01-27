struct VS_IN
{
	float3 pos : POSITION;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 col : COLOUR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float4 position = float4(input.pos, 1);
	output.pos = mul(position, VP);
	output.col = input.col;

	return output;
}