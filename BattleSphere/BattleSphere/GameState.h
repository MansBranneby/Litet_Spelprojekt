#pragma once
#include "State.h"
#include "Transparency.h"
#include "Lights.h"
#include "DBOHandler.h"
#include "CollisionTests.h"
#include "StructsAndEnums.h"
#include "Graph.h"
#include "SpawnDrone.h"
#include "BillboardHandler.h"

class GameState : public State 
{
	
private:
	Input* m_input;
	Robot** m_robots;
	std::vector<Resource*> m_resources;
	std::vector<Node*> m_nodes;
	BillboardHandler m_billboardHandler;
	Lights* m_lights;
	Transparency m_transparency;

	// Spawning
	SpawnDrone* m_spawnDrone;
	void spawnNodes();

	// Dynamic camera
	XMVECTOR m_camStartPos;
	XMVECTOR m_camStartLookAt;
	XMVECTOR m_vecToCam;
	bool m_zoomingOutToStart;
	XMVECTOR m_fOVPlanes[4]; // Bottom, Left, Top, Right
	void updateDynamicCamera(float dT);

	// Dynamic background objects
	DBOHandler* m_dboHandler;

	void handleMovement(Game* game, float dt, int id);
	void handleInputs(Game* game, float dt);

public:
	GameState(Game* game);
	virtual ~GameState();

	void pause();
	void resume();
	
	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

