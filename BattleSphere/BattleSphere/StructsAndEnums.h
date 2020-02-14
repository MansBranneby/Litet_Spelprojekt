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
	e_final
};

enum class stateType
{
	e_gameState,
	e_mainMenu,
	e_default
};

enum class activeMainMenu
{
	e_startGame,
	e_options,
	e_quit
};

enum class uiUpdate
{
	e_startAnimation,
	e_mainMenuSelection,
	e_quitAnimation
};

enum class uiType
{
	e_mainMenuSelection,
	e_mainMenuBox,
	e_static
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