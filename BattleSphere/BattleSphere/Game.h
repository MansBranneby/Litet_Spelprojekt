#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>  

#include "DX.h"
#include "GameObject.h"
#include "Input.h"
#include "PreLoader.h"


using namespace DirectX;

class Game
{
private:
	int m_nrOfPlayers;
	int m_controllerId[XUSER_MAX_COUNT];
	std::vector<GameObject> m_gameObjects;
	Input m_input;
	Robot* m_robots[XUSER_MAX_COUNT];
	std::vector<Projectile*> m_projectiles;
	std::vector<Resource*> m_resources;
	std::vector<Node*> m_nodes;

	PreLoader m_preLoader;

	void handleMovement(float dt, int id);
	void handleInputs(float dt);

	void updatePlayerStatus();
public:
	Game();

	void update(float dt);
	void updateSec();
	void draw();

	void release();
};