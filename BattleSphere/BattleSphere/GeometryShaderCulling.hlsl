struct GSInput {
	float4 pos : SV_POSITION;
	uint iD : VERTEX_ID;
};

struct GSOutput
{
	uint iD : VERTEX_ID;
};

[maxvertexcount(3)]
void GS_main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float3 camLookAt = { 0.0f, 0.0f, 1.0f };
	float3 vec1 = normalize(input[2].pos.xyz - input[0].pos.xyz);
	float3 vec2 = normalize(input[1].pos.xyz - input[0].pos.xyz);
	float3 triangleNormal = cross(vec2, vec1);
	
	// If the triangle is facing the camera, save indice IDs.
	if (dot(triangleNormal, camLookAt) > 0) 
	{
		for (int i = 0; i < 3; i++)
		{
			element.iD = input[i].iD;
			output.Append(element);
		}
	}
}