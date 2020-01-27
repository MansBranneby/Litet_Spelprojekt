#pragma once

#include <DirectXMath.h>

#include "ConstantBuffer.h"

using namespace DirectX;

struct LightData
{
	XMVECTOR position;
	XMVECTOR colour;
};

class Light
{
private:
	LightData m_lightData;

	ConstantBuffer* m_constantBuffer;

public:
	Light(XMVECTOR position, XMVECTOR colour);
	~Light();

	ConstantBuffer* getConstantuffer();
};

