#define BLOCK_SIZE 32
struct Plane
{
	float3 n; //Plane normal
	float d; // Distance to origin
};
struct Frustum {
	Plane planes[4];
};
RWStructuredBuffer<Frustum> out_Frustums : register(u0);
Plane ComputePlane(float3 p0, float3 p1, float3 p2)
{
	Plane plane;
	float3 v0 = p1 - p0;
	float3 v2 = p2 - p0;
	
	plane.n = normalize(cross(v2, v0));

	//Compute the distance to the origin using p0
	plane.d = dot(plane.n, p0);
	return plane;
}

cbuffer ScreenToViewParams : register(b1)
{
	float4x4 InverseProjection;
	float2 ScreenDimensions;
}
float4 ClipToView(float4 clip)
{
	// View space position.
	float4 view = mul(InverseProjection, clip);
	// Perspective projection.
	view = view / view.w;

	return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView(float4 screen)
{
	// Convert to normalized texture coordinates
	float2 texCoord = screen.xy / ScreenDimensions;

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}
// Global variables
cbuffer DispatchParams : register(b0)
{
	// Number of groups dispatched. (This parameter is not available as an HLSL system value!)
	uint3   numThreadGroups;
	uint padding; // implicit padding to 16 bytes.

	// Total number of threads dispatched. (Also not available as an HLSL system value!)
	// Note: This value may be less than the actual number of threads executed 
	// if the screen size is not evenly divisible by the block size.
	uint3   numThreads;
	uint padding2; // implicit padding to 16 bytes.
}
struct ComputeShaderInput {
	uint3 groupID           : SV_GroupID;           // 3D index of the thread group in the dispatch.
	uint3 groupThreadID     : SV_GroupThreadID;     // 3D index of local thread ID in a thread group.
	uint3 dispatchThreadID  : SV_DispatchThreadID;  // 3D index of global thread ID in the dispatch.
	uint  groupIndex        : SV_GroupIndex;        // Flattened local index of the thread within a thread group.
};
[numthreads(16, 16, 1)]
void CS_main(ComputeShaderInput IN)
{
	const float3 eyePos = float3(0, 0, 0);
	float4 screenSpace[4];
	screenSpace[0] = float4(IN.dispatchThreadID.xy * BLOCK_SIZE, -1.0f, 1.0f);
	// Top right point
	screenSpace[1] = float4(float2(IN.dispatchThreadID.x + 1, IN.dispatchThreadID.y) * BLOCK_SIZE, 1.0f, 1.0f);
	// Bottom left point
	screenSpace[2] = float4(float2(IN.dispatchThreadID.x, IN.dispatchThreadID.y + 1) * BLOCK_SIZE, 1.0f, 1.0f);
	// Bottom right point
	screenSpace[3] = float4(float2(IN.dispatchThreadID.x + 1, IN.dispatchThreadID.y + 1) * BLOCK_SIZE, 1.0f, 1.0f);

	float3 viewSpace[4];
	// Now convert the screen space points to view space
	for (int i = 0; i < 4; i++)
	{
		viewSpace[i] = ScreenToView(screenSpace[i]).xyz;
	}

	Frustum frustum;

	// Left plane
	frustum.planes[0] = ComputePlane(eyePos, viewSpace[2], viewSpace[0]);
	// Right plane
	frustum.planes[1] = ComputePlane(eyePos, viewSpace[1], viewSpace[3]);
	// Top plane
	frustum.planes[2] = ComputePlane(eyePos, viewSpace[0], viewSpace[1]);
	// Bottom plane
	frustum.planes[3] = ComputePlane(eyePos, viewSpace[3], viewSpace[2]);

	if (IN.dispatchThreadID.x < numThreads.x && IN.dispatchThreadID.y < numThreads.y)
	{
		uint index = IN.dispatchThreadID.x + (IN.dispatchThreadID.y * numThreads.x);
		out_Frustums[index] = frustum;
		
	}
}