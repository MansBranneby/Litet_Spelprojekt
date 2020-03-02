struct PSIn
{
	float4 pos  : SV_Position;
	float3 col : COLOR;
};

float4 PS_main(PSIn input) : SV_TARGET
{
	return float4(input.col.xyz, 1.0f);
}