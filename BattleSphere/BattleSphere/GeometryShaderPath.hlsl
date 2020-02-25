struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 posWC: WORLD_POSITION;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
};

[maxvertexcount(4)]
void GS_main(
	point float4 input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element[4];
	element[0].posWC = input[0] + float4(-0.5, 1, 0.5, 1);
	element[1].posWC = input[0] + float4(0.5, 1, 0.5, 1);
	element[2].posWC = input[0] + float4(-0.5, 1, -0.5, 1);
	element[3].posWC = input[0] + float4(0.5, 1, -0.5, 1);
	
	for (uint i = 0; i < 4; i++)
	{
		element[i].pos = mul(VP, element[i].posWC);
	}
	
	for (uint j = 0; j < 4; j++)
	{
		output.Append(element[j]);
	}
	
}