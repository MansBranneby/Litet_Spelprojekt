cbuffer deltaTime : register(b0)
{
	float dt;
};

struct particle
{
	float3 pos;
	float3 vel;
	float2 size;
	float timeLived;
	float lifeSpan;
	float3 gravDir;
	float3 color;
};

static const uint airResistance = 0.1f;

particle updateParticle(particle prevParticle)
{
	// Update particle position and velocity
	float3 acceleration = -pow(prevParticle.vel, 2) * airResistance + 9.82f * prevParticle.gravDir;
	prevParticle.vel += acceleration * dt;
	prevParticle.pos += prevParticle.vel * dt;

	// Update particle time lived
	prevParticle.timeLived += dt;

	return prevParticle;
};

/* Particle Dispatch Parameter layout
Index 0: VertexCountPerInstance
Index 1: InstanceCount
Index 2: StartVertexLocation
Index 3: StartInstanceLocation
Index 4: ThreadGroupCount
*/

static const uint particlesPerThread = 8;
RWBuffer<uint> prevParams : register(u0); // Read
RWStructuredBuffer<particle> prev : register(u1);
RWBuffer<uint> updatedParams : register(u2); // Write
RWStructuredBuffer<particle> updated : register(u3);

static const uint computationsPerThread = 8;
static const int yLimit = -5.0f;
static const uint threadAmount = 16;

[numthreads(threadAmount, 1, 1)]
void CS_main(uint3 dTID : SV_DispatchThreadID)
{
	// Each thread computes computationsPerThread amount of particles
	for (uint i = dTID.x * computationsPerThread; i < dTID.x * computationsPerThread + computationsPerThread; i++)
	{
		// Is i a particle from previous frame?
		if (i < prevParams[0])
		{
			particle p = updateParticle(prev[i]);
		
			// If particle is above yLimit, forward it to updated buffer
			if (p.pos.y > yLimit && (p.lifeSpan == 0 || p.timeLived < p.lifeSpan))
			{
				// Update amount of particles in updated param buffer
				uint outIdx;
				InterlockedAdd(updatedParams[0], 1, outIdx);

				// Add particle updated buffer
				updated[outIdx] = p;

				// Update dispatch thread group count
				if ((outIdx % (threadAmount * computationsPerThread)) == 0)
					InterlockedAdd(updatedParams[4], 1);
			}
		}
		else // If no more particles are available, exit for loop
			break;
	}
}