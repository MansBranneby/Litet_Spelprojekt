struct GSIn
{
	float4 pos : SV_POSITION;
	float3 vel : VELOCITY;
	float2 size : SIZE;
	float3 col : COLOR;
};

struct PSIn
{
	float4 pos  : SV_Position;
	float3 col : COLOR;
};

cbuffer deltaTime : register(b0)
{
	float4x4 VP;
};

cbuffer cameraPosition : register(b1)
{
	float4 camPos;
};


[maxvertexcount(4)]
void GS_main(
	point GSIn input[1], 
	inout TriangleStream< PSIn > output
)
{
	// Calculate quad vectors
	PSIn elements[4] = (PSIn[4])0;
	float3 norm = normalize(camPos.xyz - input[0].pos.xyz);
	float3 right = normalize(input[0].vel);
	float3 up = cross(right, norm);
	right *= input[0].size.x;
	up *= input[0].size.y;

	// Build quad around point
	elements[0].pos = input[0].pos + float4(- right / 2 + up / 2, 0.0f);
	elements[1].pos = elements[0].pos + float4(right, 0.0f);
	elements[2].pos = elements[1].pos + float4(-up - right, 0.0f);
	elements[3].pos = elements[2].pos + float4(right, 0.0f);

	// Send quad corners to pixel shader
	for (int j = 0; j < 4; j++)
	{
		elements[j].col = input[0].col;
		elements[j].pos = mul(VP, elements[j].pos);
		output.Append(elements[j]);
	}
}