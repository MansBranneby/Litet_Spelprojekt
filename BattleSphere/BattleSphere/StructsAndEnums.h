#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

enum class objectType {
	drone = 0,
	weapon = 1,
	robot = 2,
	node = 3,
	projectile = 4,
	resource = 5
};

struct objectData {
	XMVECTOR pos = XMVectorSet(0, 0, 0, 0);
	XMVECTOR rotation = XMVectorSet(1, 0, 0, 0);
	XMVECTOR scale = XMVectorSet(1, 1, 1, 0);
};

struct vertex {
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};