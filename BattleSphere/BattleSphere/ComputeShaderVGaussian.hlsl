Texture2D<float4> InputTex : register(t0);
RWTexture2D<float4> OutputTex : register(u0);

//Image size / 2
#define size_x 960
#define size_y 540

// Gaussian kernel
static const float filter[7] =
{
	0.030078323, 0.104983664, 0.222250419, 0.285375187, 0.222250419, 0.104983664, 0.030078323
};

[numthreads(size_x, 1, 1)]

void CS_main(uint3 DispatchThreadID : SV_DispatchThreadID)
{
	OutputTex[DispatchThreadID.xy] = float4(0.0f, 1.0f, 0.0f, 1.0f);
}