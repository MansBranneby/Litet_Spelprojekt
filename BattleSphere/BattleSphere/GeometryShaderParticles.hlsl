struct GSIn
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float3 col : COLOR;
};

struct PSIn
{
	float4 pos  : SV_Position;
	float3 col : COLOR;
};

cbuffer cameraPosition : register(b0)
{
	float4 camPos;
};

cbuffer deltaTime : register(b1)
{
	float4x4 VP;
};

[maxvertexcount(4)]
void GS_main(
	point GSIn input[1], 
	inout TriangleStream< PSIn > output
)
{
	PSIn elements[4] = (PSIn[4])0;
	float3 norm = normalize(camPos.xyz - input[0].pos.xyz);
	float3 right = cross(norm, float3(0, 1, 0));
	float3 up = cross(right, norm);
	right *= input[0].size.x;
	up *= input[0].size.y;
	elements[0].pos = input[0].pos + float4(- right / 2 + up / 2, 0.0f);
	elements[1].pos = elements[0].pos + float4(right, 0.0f);
	elements[2].pos = elements[1].pos + float4(-up - right, 0.0f);
	elements[3].pos = elements[2].pos + float4(right, 0.0f);

	for (int j = 0; j < 4; j++)
	{
		elements[j].col = input[0].col;
		elements[j].pos = mul(VP, elements[j].pos);
		output.Append(elements[j]);
	}
}