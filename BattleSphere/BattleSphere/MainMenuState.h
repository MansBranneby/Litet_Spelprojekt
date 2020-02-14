#pragma once

#include "State.h"
#include "UI_Element.h"

class MainMenuState : public State
{
private:
	std::vector<UI_Element*> m_uiElements;
	activeMainMenu m_activeMenu;
	
	float m_selectionTimer;

public:
	MainMenuState();
	virtual ~MainMenuState();

	void pause();
	void resume();

	void handleInput(Game* game);
	void update(Game* game, float dt);
	void draw(Game* game);
};

