#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"

using namespace DirectX;

#define PISTOL 0
#define RIFLE 1
#define MOVEMENT 2
#define SHIELD 3

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
	e_final
};

enum class stateType
{
	e_gameState,
	e_mainMenu,
	e_default
};

struct vertex 
{
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};

struct material
{
	XMVECTOR ambient = XMVectorSet(0.5f, 0, 0.5f, 0); // ambient.xyz, illumination model enumeration (illum)
	XMVECTOR diffuse = XMVectorSet(0.5f, 0, 0.5f, 0); // diffuse.xyz, refraction (Ni)
	XMVECTOR specular = XMVectorSet(0.5f, 0, 0.5f, 0); // specular.xyz, shininess (Ns)
	XMVECTOR emission = XMVectorSet(0.5f, 0, 0.5f, 0); // emission.xyz, opacity (d)
};

struct objectData
{
	XMVECTOR pos = XMVectorSet(0, 0, 0, 0);
	XMVECTOR rotation = XMVectorSet(1, 0, 0, 0);
	XMVECTOR scale = XMVectorSet(1, 1, 1, 0);
	material material;
};

// Game update return data
struct returnInfo
{
	float x, y, z;
};

struct vertexAndId
{
	float posX, posY, posZ;
	unsigned int iD;
};