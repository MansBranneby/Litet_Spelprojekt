#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

enum class objectType
{
	e_drone = 0,
	e_weapon = 1,
	e_robot = 2,
	e_node = 3,
	e_projectile = 4,
	e_resource = 5,
	e_scene = 6
};

enum class renderPass
{
	e_scene,
	e_downSample,
	e_final,
	e_opaque,
	e_transparent
};

enum class stateType
{
	e_gameState,
	e_mainMenu,
	e_default
};

struct objectData 
{
	XMVECTOR pos = XMVectorSet(0, 0, 0, 0);
	XMVECTOR rotation = XMVectorSet(1, 0, 0, 0);
	XMVECTOR scale = XMVectorSet(1, 1, 1, 0);
};

struct vertex 
{
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};

// Game update return data
struct returnInfo
{
	float x, y, z;
};