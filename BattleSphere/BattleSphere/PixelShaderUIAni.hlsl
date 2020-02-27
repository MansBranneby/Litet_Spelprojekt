Texture2D elementTex : register(t0);

SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float alpha : ALPHA;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 aniTimer;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	static const float PI = 3.14159265f;

	float4 elementColour = elementTex.Sample(sampAni, input.tex).xyzw;
	
	float x = cos(aniTimer.x);
	float y = sin(aniTimer.x);

	float u = input.tex.x * 2 - 1;
	float v = -(input.tex.y * 2 - 1);
	
	float uCos = acos(u);
	float vSin = asin(v);

	if (atan2(v, u) > 0.0f)
	{
		if (aniTimer.x < PI)
		{
			if (atan2(v, u) > aniTimer.x)
				elementColour.xyz = float3(0.89f, 0.1f, 0.7f);
			else
				elementColour.xyz = float3(0.0f, 1.0f, 0.623f);
		}
		else
		{
			if (atan2(v, u) < aniTimer.x - PI)
				elementColour.xyz = float3(0.89f, 0.1f, 0.7f);
			else
				elementColour.xyz = float3(0.0f, 1.0f, 0.623f);
		}
	}
	else
	{
		if (aniTimer.x > PI)
		{
			if (2 * PI + atan2(v, u) > aniTimer.x)
				elementColour.xyz = float3(0.89f, 0.1f, 0.7f);
			else
				elementColour.xyz = float3(0.0f, 1.0f, 0.623f);
		}
		else
		{
			if (2 * PI + atan2(v, u) < aniTimer.x + PI)
				elementColour.xyz = float3(0.89f, 0.1f, 0.7f);
			else
				elementColour.xyz = float3(0.0f, 1.0f, 0.623f);
		}
	}
	
	elementColour.w *= input.alpha;

	return elementColour;
};