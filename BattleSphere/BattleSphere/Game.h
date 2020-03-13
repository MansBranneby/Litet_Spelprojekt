#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <stdlib.h> 

#include "DX.h"
#include "ProjectileBank.h"
#include "GameObject.h"
#include "Input.h"
#include "PreLoader.h"
#include "State.h"
#include "QuadtreeNode.h"
#include "Graph.h"
#include "Robot.h"

using namespace DirectX;

class State;
class Game
{
	
private:
	std::vector<State*> m_states;

	int m_nrOfPlayers;
	int m_playerId[XUSER_MAX_COUNT];
	Input m_input;
	Robot* m_robots[XUSER_MAX_COUNT];
	
	PreLoader m_preLoader;
	QuadtreeNode* m_quadtree;

public:
	
	Game();

	bool update(float dt);
	//void updateSec();
	void draw(renderPass pass = renderPass::e_scene);
	void pushState(State* state);
	void changeState(stateType state);
	bool isActive(stateType state);
	int getPlayerId(int robotNr);
	int setPlayerIdIndex(int id);
	int getPlayerIdIndex(int id);
	int getNrOfPlayers() const;
	void leavePlayerIdIndex(int id);
	void updatePlayerStatus();
	Robot** getRobots();
	Input* getInput();
	PreLoader* getPreLoader();
	QuadtreeNode* getQuadtree();

	void release();
};