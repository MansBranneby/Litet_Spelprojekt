Texture2D<float4> InputTex : register(t0);
RWTexture2D<float4> OutputTex : register(u0);

//Image size / 2
#define size_x 32
#define size_y 30

// Gaussian kernel
static const float kernel[7] =
{
	0.030078323, 0.104983664, 0.222250419, 0.285375187, 0.222250419, 0.104983664, 0.030078323
};

//static const float kernel[29] =
//{
//	0.001607, 0.002753, 0.004531, 0.007167, 0.010893, 0.015908, 0.022325, 0.030106, 0.039011, 0.048575, 0.05812, 0.066823, 0.073827, 0.078376, 0.079954, 0.078376, 0.073827, 0.066823, 0.05812, 0.048575, 0.039011, 0.030106, 0.022325, 0.015908, 0.010893, 0.007167, 0.004531, 0.002753, 0.001607
//};
//
//static const float kernel[59] =
//{
//	0.001821, 0.002219, 0.002686, 0.003228, 0.003853, 0.004567, 0.005376, 0.006285, 0.007296, 0.008411, 0.00963, 0.01095, 0.012364, 0.013864, 0.015439, 0.017073, 0.01875, 0.02045, 0.022149, 0.023823, 0.025447, 0.026993, 0.028435, 0.029748, 0.030905, 0.031886, 0.03267, 0.033241, 0.033589, 0.033706, 0.033589, 0.033241, 0.03267, 0.031886, 0.030905, 0.029748, 0.028435, 0.026993, 0.025447, 0.023823, 0.022149, 0.02045, 0.01875, 0.017073, 0.015439, 0.013864, 0.012364, 0.01095, 0.00963, 0.008411, 0.007296, 0.006285, 0.005376, 0.004567, 0.003853, 0.003228, 0.002686, 0.002219, 0.001821
//};

//static const float kernel[99] =
//{
//	0.001273, 0.001421, 0.001582, 0.001758, 0.001949, 0.002156, 0.002379, 0.00262, 0.002878, 0.003155, 0.003451, 0.003766, 0.0041, 0.004453, 0.004827, 0.005219, 0.005631, 0.006062, 0.006511, 0.006977, 0.007459, 0.007957, 0.008469, 0.008993, 0.009529, 0.010073, 0.010624, 0.01118, 0.011739, 0.012297, 0.012853, 0.013403, 0.013946, 0.014477, 0.014995, 0.015496, 0.015978, 0.016437, 0.016871, 0.017278, 0.017654, 0.017998, 0.018306, 0.018578, 0.018811, 0.019004, 0.019155, 0.019264, 0.01933, 0.019352, 0.01933, 0.019264, 0.019155, 0.019004, 0.018811, 0.018578, 0.018306, 0.017998, 0.017654, 0.017278, 0.016871, 0.016437, 0.015978, 0.015496, 0.014995, 0.014477, 0.013946, 0.013403, 0.012853, 0.012297, 0.011739, 0.01118, 0.010624, 0.010073, 0.009529, 0.008993, 0.008469, 0.007957, 0.007459, 0.006977, 0.006511, 0.006062, 0.005631, 0.005219, 0.004827, 0.004453, 0.0041, 0.003766, 0.003451, 0.003155, 0.002878, 0.00262, 0.002379, 0.002156, 0.001949, 0.001758, 0.001582, 0.001421, 0.001273
//};

static const int kernelHS = 3;
static const int kernelSize = 7;

groupshared float4 horizontalPoints[size_y + kernelSize - 1];


struct CS_input
{
	uint3 DispatchThreadID : SV_DispatchThreadID;
	uint3 GroupThreadID : SV_GroupThreadID;
};

[numthreads(1, size_y, 1)]

void CS_main(CS_input input)
{
	float4 data = InputTex.Load(input.DispatchThreadID);
	horizontalPoints[input.GroupThreadID.y + kernelHS] = data;
	if (input.GroupThreadID.y == 0)
	{
		for (int i = 0; i < kernelHS; i++)
		{
			horizontalPoints[i] = InputTex.Load(float3(input.DispatchThreadID.x, input.DispatchThreadID.y - kernelHS + i, input.DispatchThreadID.z));
			horizontalPoints[size_y + kernelHS + i] = InputTex.Load(float3(input.DispatchThreadID.x, input.DispatchThreadID.y + size_x + i, input.DispatchThreadID.z));
		}
	}
	GroupMemoryBarrierWithGroupSync(); // Sync threads

	float4 colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < kernelSize; i++)
		colour += horizontalPoints[i + input.GroupThreadID.y] * kernel[i];

	OutputTex[input.DispatchThreadID.xy] = colour;
}