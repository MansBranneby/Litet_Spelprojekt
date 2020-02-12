#pragma once

#include "State.h"

class MainMenuState : public State
{
private:

public:
	MainMenuState();
	virtual ~MainMenuState();

	void pause();
	void resume();

	void handleInput(Game* game);
	void update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

