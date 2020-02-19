#pragma once

#include "State.h"
#include "Transparency.h"
#include "Lights.h"

#define START_SPAWNED_RESOURCES 0
#define MAX_RESOURCES_OUT_PER_PLAYER 35 // Default 2
#define SPAWN_INTERVAL 3.0f
#define SPECIAL_RESOURCE_CHANCE 25 // % chance

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
	float m_collectedTime;
	int m_normalSpawnAmount;
	int m_specialSpawnAmount;
	void startSpawn();
	int getSpawnIndex();
	int getSpecialSpawnIndex();

	void handleMovement(Game* game, float dt, int id);
	void handleInputs(Game* game, float dt);

public:
	GameState();
	virtual ~GameState();

	void regularSpawn(float dT);

	void pause();
	void resume();
	
	void handleInput(Game* game);
	void update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

