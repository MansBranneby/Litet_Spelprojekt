Texture2D elementTex : register(t0);

SamplerState sampAni;

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 colour;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float alpha : ALPHA;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	static const float PI = 3.14159265f;
	float4 elementColour = elementTex.Sample(sampAni, input.tex).xyzw;
	
	if (colour.w == -1.0f)
	{
		if (input.pos.x > colour.x / 2.0f + colour.x / 15.0f)
			elementColour.w = 0.0f;
		if (input.pos.x < colour.x / 2.0f - colour.x / 15.0f)
			elementColour.w = 0.0f;
	}
	else if (colour.x != 0.0f || colour.y != 0.0f || colour.z != 0.0f)
		elementColour.xyz = colour.xyz;
	else
	{
		if (colour.w != 0.0f &&  colour.w != -1.0f)
		{
			float u = input.tex.x * 2 - 1;
			float v = -(input.tex.y * 2 - 1);
			float uvAngle;
			float cdAngle = colour.w * 2 * PI;
			float cd = colour.w;
			if (cd > 0.75f)
			{
				uvAngle = atan(v / u);
				if (u > 0.0f && v > 0.0f && uvAngle > cdAngle - 3.0f * PI / 2.0f)
					elementColour.xyz = elementColour.xyz;
				else
					elementColour.xyz = elementColour.xyz * 0.2f;
			}
			else if (cd > 0.5f && cd <= 0.75f)
			{
				uvAngle = atan(u / -v);
				if (u > 0.0f && v > 0.0f)
					elementColour.xyz = elementColour.xyz;
				else if (u > 0.0f && v < 0.0f && uvAngle > cdAngle - PI)
					elementColour.xyz = elementColour.xyz;
				else
					elementColour.xyz = elementColour.xyz * 0.2f;
			}
			else if (cd > 0.25f && cd <= 0.5f)
			{
				uvAngle = atan(-v / -u);
				if ((u > 0.0f && v > 0.0f) || (u > 0.0f && v < 0.0f))
					elementColour.xyz = elementColour.xyz;
				else if (u < 0.0f && v < 0.0f && uvAngle > cdAngle - PI / 2.0f)
					elementColour.xyz = elementColour.xyz;
				else
					elementColour.xyz = elementColour.xyz * 0.2f;
			}
			else
			{
				uvAngle = atan(-u / v);
				if ((u > 0.0f && v > 0.0f) || (u > 0.0f && v < 0.0f) || (u < 0.0f && v < 0.0f))
					elementColour.xyz = elementColour.xyz;
				else if (u < 0.0f && v > 0.0f && uvAngle > cdAngle)
					elementColour.xyz = elementColour.xyz;
				else
					elementColour.xyz = elementColour.xyz * 0.2f;
			}

			/*
			if (u > 0.0f)
			{
				uvAngle = atan2(u, -v);
				if (uvAngle < cdAngle)
					elementColour.xyz = elementColour.xyz * 0.2f;

			}
			else
			{
				uvAngle = atan2(u, v);
				if (uvAngle < cdAngle)
					elementColour.xyz = elementColour.xyz * 0.2f;

			}*/
		}
	}
	
	elementColour.w *= input.alpha;
	return elementColour;
};