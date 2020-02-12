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
#include "State.h"

using namespace DirectX;

class State;
class Game
{
	
private:
	std::vector<State*> m_states;

	int m_nrOfPlayers;
	int m_controllerId[XUSER_MAX_COUNT];
	Input m_input;
	Robot* m_robots[XUSER_MAX_COUNT];

	PreLoader m_preLoader;

public:
	
	Game();

	void update(float dt);
	//void updateSec();
	void draw();
	void pushState(State* state);
	void changeState(stateType state);
	bool isActive(stateType state);
	
	void updatePlayerStatus();
	Robot** getRobots();
	Input* getInput();
	PreLoader* getPreLoader();

	void release();
};