struct particle
{
	float3 pos : POSITION;
	float3 vel : VELOCITY;
	float2 size : SIZE;
	float3 col : COLOR;
};

struct GSIn
{
	float4 pos : SV_POSITION;
	float2 size : SIZE;
	float3 col : COLOR;
};

StructuredBuffer<particle> particles : register(t0);

GSIn VS_main(uint vID : SV_VertexID)
{
	GSIn output = (GSIn)0;
	output.pos = float4(particles[vID].pos, 1);
	output.size = particles[vID].size;
	output.col = particles[vID].col;

	return output;
}