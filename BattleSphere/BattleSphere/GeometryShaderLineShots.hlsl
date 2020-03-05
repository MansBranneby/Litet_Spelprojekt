struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 posWC: WORLD_POSITION;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix VP;
};

cbuffer GS_CONSTANT_BUFFER_2 : register(b1)
{
	float width;
	float alpha;
};

[maxvertexcount(4)]
void GS_main(
	line float4 input[2] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	float3 direction = normalize(input[1].xyz - input[0].xyz);
	float4 right = float4(cross(direction, float3(0, 1, 0)), 0.0f) * width;//or left idk lol left handed system :) :gun:

	GSOutput element[4];
	element[0].posWC = input[1] - right;//+ float4(-1, 0, 1, 0);//-right;
	element[1].posWC = input[0] - right;//+ float4(1, 0, 1, 0);//+right;
	element[2].posWC = input[1] + right;//+ float4(-1, 0, -1, 0);//-right;
	element[3].posWC = input[0] + right;//+ float4(1, 0, -1, 0);//+right;

	for (uint i = 0; i < 4; i++)
	{
		element[i].pos = mul(VP, float4(element[i].posWC.xyz, 1.0f));
	}

	for (uint j = 0; j < 4; j++)
	{
		output.Append(element[j]);
	}

}