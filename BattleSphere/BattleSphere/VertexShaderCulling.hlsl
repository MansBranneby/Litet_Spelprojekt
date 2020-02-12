struct VS_IN
{
	float3 pos : POSITION;
	uint iD : VERTEX_ID;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	uint iD : VERTEX_ID;
};

cbuffer VS_CONSTANT_BUFFER : register(b1)
{
	matrix MV;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = (mul(MV, float4(input.pos, 1.0f)));
	output.iD = input.iD;

	return output;
}