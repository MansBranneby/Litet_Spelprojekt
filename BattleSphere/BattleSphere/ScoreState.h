#pragma once
#include "State.h"
#include "UI_Element.h"

class ScoreState : public State
{
private:
	std::vector<UI_Element*> m_uiElements;
	std::vector<UI_Element*> m_uiElements_numbers;

public:
	ScoreState();
	~ScoreState();

	void pause();
	void resume();

	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};