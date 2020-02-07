#define BLOCK_SIZE 4
#define NUM_LIGHTS 1024

struct Sphere 
{
	float3 c;
	float r;
};
struct Cone 
{
	float3 tip;
	float height;
	float3 direction;
	float radius;
};
struct Plane
{
	float3 n; //Plane normal
	float d; // Distance to origin
};
cbuffer DispatchParams : register(b0)
{
	uint3   numThreadGroups;
	uint padding; 
	uint3   numThreads;
	uint padding2; 
}
cbuffer VIEW_CBUFFER : register(b1)
{
	matrix V;
};

struct Frustum 
{
	Plane planes[4];
};
struct Light 
{
	int Enabled;
	int Type;
	float Range, SpotlightAngle;
	float4 Position;
	float3 Direction;
	float Intensity;
	float4 color;
};
bool PointInsidePlane(float3 p, Plane plane)
{
	return dot(plane.n, p) - plane.d < 0;
}

bool ConeInsidePlane(Cone cone, Plane plane)
{
	// Compute the farthest point on the end of the cone to the positive space of the plane.
	float3 m = cross(cone.direction, cross(cone.direction, plane.n));
	float3 Q = cone.tip + cone.direction * cone.height - m * cone.radius;

	
	return PointInsidePlane(cone.tip, plane) && PointInsidePlane(Q, plane);
}
bool ConeInsideFrustum(Cone cone, Frustum frustum)
{
	bool result = true;


	// Then check frustum planes
	for (int i = 0; i < 4 && result; i++)
	{
		if (ConeInsidePlane(cone, frustum.planes[i]))
		{
			result = false;
		}
	}

	return result;
}
bool SphereInsidePlane(Sphere sphere, Plane plane) 
{
	return dot(plane.n, sphere.c) - plane.d < -sphere.r;
}
bool SphereInsideFrustum(Sphere sphere, Frustum frustum) 
{
	for (int i = 0; i < 4; i++)
	{
		if (SphereInsidePlane(sphere, frustum.planes[i]))
			return false;
	}
	return true;
}
groupshared uint uMinDepth;
groupshared uint uMaxDepth;
groupshared uint o_LightCount;
groupshared uint o_LightIndexStartOffset;
groupshared uint o_LightList[1024];
groupshared Frustum GroupFrustum;
void o_AppendLight(uint lightIndex)
{
	uint index; // Index into the visible lights array.
	InterlockedAdd(o_LightCount, 1, index);
	if (index < 1024)
	{
		o_LightList[index] = lightIndex;
	}
}
StructuredBuffer<Frustum> in_Frustums : register (t0);
StructuredBuffer<Light> Lights : register (t1);
RWStructuredBuffer<uint> o_LightIndexCounter : register(u0);
RWStructuredBuffer<uint> o_LightIndexList : register(u1);
RWTexture2D<uint2> o_LightGrid : register(u2);

struct ComputeShaderInput 
{
	uint3 groupID           : SV_GroupID;           // 3D index of the thread group in the dispatch.
	uint3 groupThreadID     : SV_GroupThreadID;     // 3D index of local thread ID in a thread group.
	uint3 dispatchThreadID  : SV_DispatchThreadID;  // 3D index of global thread ID in the dispatch.
	uint  groupIndex        : SV_GroupIndex;        // Flattened local index of the thread within a thread group.
};
[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void CS_main(ComputeShaderInput IN)
{
	if (IN.dispatchThreadID.x == 0 && IN.dispatchThreadID.y == 0)
	{
		o_LightIndexCounter[0] = 0;
	}
	GroupMemoryBarrierWithGroupSync();
	int2 texCoord = IN.dispatchThreadID.xy;

	if (IN.groupIndex == 0) // Avoid contention by other threads in the group.
	{
		o_LightCount = 0;
		GroupFrustum = in_Frustums[IN.groupID.x + (IN.groupID.y * numThreads.x)];
	}


	for (uint i = IN.groupIndex; i < NUM_LIGHTS; i += BLOCK_SIZE * BLOCK_SIZE)
	{
		if (Lights[i].Enabled)
		{
			Light light = Lights[i];

			switch (light.Type)
			{
			case 0: //Point light
			{

				Sphere sphere = { mul(V, light.Position).xyz, light.Range };
				if (SphereInsideFrustum(sphere, GroupFrustum))
				{
					// Add light to light list for opaque geometry.
					o_AppendLight(i);
				}
			}
			break;
			case 1: //Directional light
			{
				o_AppendLight(i);
			}
			break;
			case 2:
			{
				float coneRadius = tan(radians(light.SpotlightAngle)) * light.Range;
				Cone cone = { mul(V, light.Position).xyz, light.Range, mul(V, float4(normalize(light.Direction.xyz), 0.0f)).xyz, coneRadius };
				if (ConeInsideFrustum(cone, GroupFrustum))
				{

						// Add light to light list for opaque geometry.
						o_AppendLight(i);

				}
			}
			break;
			}
		}
		else
		{
			break;
		}
	}
	GroupMemoryBarrierWithGroupSync();
	if (IN.groupIndex == 0)
	{
		// Update light grid for opaque geometry.
		
		InterlockedAdd(o_LightIndexCounter[0], o_LightCount, o_LightIndexStartOffset);
		o_LightGrid[IN.groupID.xy] = uint2(o_LightIndexStartOffset, o_LightCount);
		
	}

	GroupMemoryBarrierWithGroupSync();
	for (i = IN.groupIndex; i < o_LightCount; i += BLOCK_SIZE * BLOCK_SIZE)
	{
		o_LightIndexList[o_LightIndexStartOffset + i] = o_LightList[i];
	}

}