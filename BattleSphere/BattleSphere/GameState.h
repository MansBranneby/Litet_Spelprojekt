#pragma once

#include "State.h"
#include "Transparency.h"
#include "Lights.h"
class GameState : public State 
{
private:
	Lights* m_lights;
	Transparency m_transparency;
	
	std::vector<GameObject> m_gameObjects;
	std::vector<Projectile*> m_projectiles;

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

