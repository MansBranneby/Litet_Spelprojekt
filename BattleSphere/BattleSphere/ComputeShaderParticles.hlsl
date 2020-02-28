#define MOVEMENT_SPEED 20.0f

cbuffer deltaTime : register(b1)
{
	float dt;
};

struct particle 
{
	float3 pos;
	float3 vel;
	float2 size;
	float3 color;
};

static const uint particlesPerThread = 8
RWStructuredBuffer<particle> input : register(u0); // Read from 0
RWStructuredBuffer<particle> output : register(u1); // Write to 1

[numthreads(16, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
	particle temp = inputBuffer.Load(DTid.x);
	temp.pos.y -= MOVEMENT_SPEED * dt;
	if (temp.pos.y < -2)
		temp.pos.y += 100.0f;
	outputBuffer[DTid.x] = temp;
}