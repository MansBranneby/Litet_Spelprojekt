struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 posWC: WORLD_POSITION;
};

cbuffer PS_CONST_BUFFER : register (b0)
{
	float4 pulsePos;
}

cbuffer PS_CONST_BUFFER_2 : register (b1)
{
	float4 colour;
}

float4 PS_main(GSOutput input) : SV_TARGET
{
	float alpha = max(0.0f, 30 / pow(distance(pulsePos, input.posWC), 1.8f));
	
	return float4(colour.xyz, min(1.0f, alpha));
}