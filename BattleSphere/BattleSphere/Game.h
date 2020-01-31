#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

#include "DX.h"
#include "GameObject.h"
#include "Input.h"
#include "Robot.h"
#include "PreLoader.h"


using namespace DirectX;

class Game
{
private:
	int m_nrOfPlayers;
	std::vector<GameObject> m_gameObjects;
	Input input;
	std::vector<Robot> m_robots;

	PreLoader preLoader;

	void handleMovement(float dt, int id);
	void handleInputs(float dt);
public:
	Game();

	void update(float dt);
	void updateSec();
	void draw();

	void release();
};