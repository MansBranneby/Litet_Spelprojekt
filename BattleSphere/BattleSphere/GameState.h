#pragma once
#include "State.h"
#include "Transparency.h"
#include "Lights.h"
#include "DBOHandler.h"
#include "CollisionTests.h"
#include "StructsAndEnums.h"
#include "Graph.h"
#include "SpawnDrone.h"
#include "Particles.h"
#include "Sound.h"
#include "Node.h"
#include "LineShots.h"
#include "BillboardHandler.h"
#include "UserInterface.h"
#include <math.h>

class GameState : public State 
{
	
private:
	Input* m_input;
	Robot** m_robots;
	std::vector<Resource*> m_resources;
	std::vector<Node*> m_nodes;
	BillboardHandler m_billboardHandler;
	Lights* m_lights;
	int m_BSPDLightIndex[2];
	float m_BSPDtimer;
	Transparency m_transparency;
	LineShots m_lineShots;
	UserInterface* m_userInterface;

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
	bool m_devZoomOut;

	// BSPD lights
	void bspdLightUpdate(float dt);

	bool m_updateMission;

	// Dynamic background objects
	DBOHandler* m_dboHandler;

	// Particles
	Particles m_particles;

	// Sound
	float m_sawInterval;

	void handleMovement(Game* game, float dt, int id);
	void handleInputs(Game* game, float dt);

	// Quit Game
	bool m_quitGame;

public:
	GameState(Game* game);
	~GameState();

	void pause();
	void resume();
	
	void firstTimeSetUp(Game* game);
	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

