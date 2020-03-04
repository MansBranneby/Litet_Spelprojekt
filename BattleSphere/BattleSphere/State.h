#pragma once
#include "Game.h"

class Game;
class State
{
private:
	bool m_paused;

protected:
	stateType m_type;

public:
	State();
	virtual ~State();
	
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void firstTimeSetUp(Game* game) = 0;
	virtual void handleInput(Game* game) = 0;
	virtual bool update(Game* game, float dt) = 0;
	virtual void draw(Game* game, renderPass pass = renderPass::e_scene) = 0;

	void activateState(Game* game, stateType state);
	stateType getType();
	void setPaused(bool paused);

	bool isPaused();
};