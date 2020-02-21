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
	
	int m_readyState[XUSER_MAX_COUNT];
	float m_selectionTimer;
	int m_robotColour[4] = { -1, -1, -1, -1 };
	int m_availableColours[5];

	bool handleInputs(Game* game, float dt);
	void changeColour(Game* game, int i, bool dir);

public:
	MainMenuState();
	virtual ~MainMenuState();

	bool hi_mainMenu(Game* game);
	void hi_robotSelection(Game* game);
	void hi_options(Game* game);

	void u_mainMenu(Game* game, float dt);
	void u_robotSelection(Game* game, float dt);
	void u_options(Game* game, float dt);

	void pause();
	void resume();

	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

