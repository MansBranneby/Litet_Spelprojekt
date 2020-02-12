#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

#include "DX.h"
#include "GameObject.h"
#include "Input.h"
#include "PreLoader.h"
#include "QuadtreeNode.h"

using namespace DirectX;
struct returnInfo
{
	float x, y, z;
};
class Game
{
	
private:
	int m_nrOfPlayers;
	int m_controllerId[XUSER_MAX_COUNT];
	std::vector<GameObject*> m_gameObjects;
	Input m_input;
	Robot* m_robots[XUSER_MAX_COUNT];
	std::vector<Projectile*> m_projectiles;

	PreLoader m_preLoader;
	QuadtreeNode* m_QuadTreeRoot;
	void handleMovement(float dt, int id);
	void handleInputs(float dt);

	void updatePlayerStatus();
public:
	
	Game();

	returnInfo update(float dt);
	void updateSec();
	void draw();

	void release();
};