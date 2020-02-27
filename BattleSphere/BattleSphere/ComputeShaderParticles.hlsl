#define MOVEMENT_SPEED 20.0f

cbuffer deltaTime {
	float dt;
};

struct particle {
	float3 pos;
	float2 tex;
	float3 normal;
};

StructuredBuffer<particle> inputBuffer : register(t0);
RWStructuredBuffer<particle> outputBuffer : register(u0);

[numthreads(1000, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
	particle temp = inputBuffer.Load(DTid.x);
	temp.pos.y -= MOVEMENT_SPEED * dt;
	if (temp.pos.y < -2)
		temp.pos.y += 100.0f;
	outputBuffer[DTid.x] = temp;
}