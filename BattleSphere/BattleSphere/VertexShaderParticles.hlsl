struct particle
{
	float3 pos : POSITION;
	float3 vel : VELOCITY;
	float2 size : SIZE;
	float timeLived : TIME_LIVED;
	float lifeSpan : LIFESPAN;
	float3 gravDir : GRAVITY_DIRECTION;
	float3 col : COLOR;
};

struct GSIn
{
	float4 pos : SV_POSITION;
	float3 vel : VELOCITY;
	float2 size : SIZE;
	float3 col : COLOR;
};

StructuredBuffer<particle> particles : register(t0);

GSIn VS_main(uint vID : SV_VertexID)
{
	GSIn output = (GSIn)0;
	output.pos = float4(particles[vID].pos, 1.0f);
	output.vel = particles[vID].vel;
	// Change size according to time lived, constantly making particles smaller
	float sizeModifier = 1.0f;
	if (particles[vID].lifeSpan != 0.0f)
		sizeModifier = max(((particles[vID].lifeSpan - particles[vID].timeLived) / particles[vID].lifeSpan), 0.0f);
	output.size = particles[vID].size * sizeModifier;
	output.col = particles[vID].col;

	return output;
}