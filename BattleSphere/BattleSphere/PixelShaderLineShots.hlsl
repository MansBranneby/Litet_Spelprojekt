struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 posWC: WORLD_POSITION;
};

cbuffer PS_CONST_BUFFER : register (b0)
{
	float4 colour;
}

cbuffer PS_CONSTANT_BUFFER_2 : register(b1)
{
	float width;
	float alpha;
};

float4 PS_main(GSOutput input) : SV_TARGET
{
	return float4(colour.xyz, alpha);
}