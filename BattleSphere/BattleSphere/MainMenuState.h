#pragma once

#include "State.h"
#include "UI_Element.h"

#define SELECTIONSPEED 1000.0f

enum MenuState
{
	e_mainMenu,
	e_robotSelection,
	e_optionsMenu
};
enum ActiveMainMenu
{
	e_startGame,
	e_options,
	e_quit
};

class MainMenuState : public State
{
private:
	std::vector<UI_Element*> m_uiElements;
	MenuState m_menuState;
	ActiveMainMenu m_activeMenu;
	
	float m_selectionTimer;

public:
	MainMenuState();
	virtual ~MainMenuState();

	void hi_mainMenu(Game* game);
	void hi_robotSelection(Game* game);
	void hi_options(Game* game);

	void pause();
	void resume();

	void handleInput(Game* game);
	void update(Game* game, float dt);
	void draw(Game* game, int index);
};

