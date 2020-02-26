struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 posWC: WORLD_POSITION;
};
cbuffer PS_CONST_BUFFER : register (b0)
{
	float4 pulsePos;
}
float4 PS_main(GSOutput input) : SV_TARGET
{
	float alpha = max(0.0f, 30 / pow(distance(pulsePos, input.posWC), 1.8f));
	return float4(1.0f, 0.0f, 1.0f, alpha);
}