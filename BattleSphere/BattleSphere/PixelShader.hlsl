struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 col : COLOUR;
};

float4 PS_main(PS_IN input) : SV_Target
{
	return float4(input.col, 1.0f);
};