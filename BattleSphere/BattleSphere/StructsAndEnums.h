#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include <vector>
#include <cmath>

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
	e_final
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

struct boundingData {
	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT2 halfWD = XMFLOAT2(0, 0);
	XMVECTOR xAxis = XMVectorSet(0, 0, 0, 0);
	XMVECTOR zAxis = XMVectorSet(0, 0, 0, 0);
};

struct vectorPairProjections {
	XMVECTOR vec1 = XMVectorSet(1, 0, 0, 0); // Vector
	XMVECTOR vec2 = XMVectorSet(0, 0, 1, 0); // Vector
	float minLength1 = 0;	// Vertex projection length
	float maxLength1 = 0;	// Vertex projection length
	float minLength2 = 0;	// Vertex projection length
	float maxLength2 = 0;	// Vertex projection length
};