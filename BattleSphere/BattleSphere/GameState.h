#pragma once

#include "State.h"
#include "Transparency.h"
#include "Lights.h"

class GameState : public State 
{
	
private:
	Input* m_input;
	Robot** m_robots;
	std::vector<Resource*> m_resources;
	std::vector<Node*> m_nodes;
	Lights* m_lights;
	Transparency m_transparency;

	void handleMovement(Game* game, float dt, int id);
	void handleInputs(Game* game, float dt);

public:
	GameState();
	virtual ~GameState();

	void pause();
	void resume();
	
	void handleInput(Game* game);
	void update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

