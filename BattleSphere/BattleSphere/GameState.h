#pragma once
#include "State.h"
#include "Transparency.h"
#include "CollisionTests.h"
#include "StructsAndEnums.h"

class GameState : public State 
{
	
private:
	Input* m_input;
	Robot** m_robots;
	std::vector<Resource*> m_resources;
	std::vector<Node*> m_nodes;
	Lights* m_lights;
	Transparency m_transparency;

	// Resource spawning lists
	std::vector<XMFLOAT2> m_spawns;
	std::vector<bool> m_freeSpawns;
	void loadLists();

	// Spawning

	int m_normalSpawnAmount;
	int m_specialSpawnAmount;
	void startSpawn();
	int getSpawnIndex();
	int getSpecialSpawnIndex();
	void spawnNodes();

	// Drone resource spawning
	int m_droneLightIndex;
	int m_spawnDroneState;
	int m_heldResourceIndex;
	XMVECTOR m_transportDestination;
	XMVECTOR m_transportDirection;
	XMVECTOR m_travelTarget;
	XMVECTOR m_travelDirection;
	float m_collectedTime;
	bool m_spawnDroneTravelling;
	bool m_spawnDroneRotating;
	GameObject m_spawnDroneBody;
	GameObject m_spawnDronePropeller[4];
	void setTravelTarget(XMVECTOR target);
	void setRotationTarget(XMVECTOR target);
	bool travelAndCheck(float dT, bool fastTravel);
	bool assignMission();
	void updateSpawnDrone(float dT);

	void handleMovement(Game* game, float dt, int id);
	void handleInputs(Game* game, float dt);

public:
	GameState();
	virtual ~GameState();

	

	void pause();
	void resume();
	
	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

