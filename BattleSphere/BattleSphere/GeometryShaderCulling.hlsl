struct GSInput {
	float4 pos : SV_POSITION;
	uint iD : TRIANGLE_ID;
};

struct GSOutput
{
	uint iD : TRIANGLE_ID;
};

[maxvertexcount(3)]
void GS_main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float3 camLookAt = { 0.0f, 0.0f, 1.0f };
	float3 triangleNormal = cross(input[1].pos - input[0].pos, input[2].pos - input[0].pos);

	if (dot(triangleNormal, camLookAt) < 0) // If the triangle is facing the camera, save it's ID.
	{
		element.iD = input[0].iD;
		output.Append(element);
	}
}