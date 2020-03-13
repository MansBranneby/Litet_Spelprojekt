#pragma once

#include "State.h"
#include "UI_Element.h"

#define SELECTIONSPEED 1000.0f

const XMVECTOR ROBOT_START_POS[4]
{
	XMVectorSet(-85.0f, 2.0f, 50.0f, 0),
	XMVectorSet(100.0f, 2.0f, -50.0f, 0),
	XMVectorSet(-85.0f, 2.0f, -50.0f, 0),
	XMVectorSet(120.0f, 2.0f, 50.0f, 0)
};

enum class MenuState
{
	e_mainMenu,
	e_robotSelection,
	e_optionsMenu
};
enum class ActiveMainMenu
{
	e_startGame,
	e_options,
	e_quit
};

enum class ActiveOptionsMenu
{
	e_resolution,
	e_fullscreen,
	e_music,
	e_apply
};

class MainMenuState : public State
{
private:
	std::vector<UI_Element*> m_uiElements;
	std::vector<UI_Element*> m_optionElements;
	std::vector<UI_Element*> m_resolutionElements;
	std::vector<XMFLOAT2> m_resolutions;
	bool m_fullscreen;
	bool m_musicOn;
	int m_selectedResIndex;
	bool m_originalFullscreenSetting;
	XMFLOAT2 m_originalResolutionSetting;

	UI_Element* m_botElements[4];
	UI_Element* m_startElement;
	MenuState m_menuState;
	ActiveMainMenu m_activeMenu;
	ActiveOptionsMenu m_optionsMenu;
	
	int m_readyState[XUSER_MAX_COUNT];
	float m_selectionTimer;
	int m_robotColour[4] = { -1, -1, -1, -1 };
	int m_availableColours[5];

	bool handleInputs(Game* game, float dt);
	void changeColour(Game* game, int i, bool dir);
	void adjustElementsForScreen();

	void leaveColour(int robotNr);
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
	void reset();

	void firstTimeSetUp(Game* game);
	void handleInput(Game* game);
	bool update(Game* game, float dt);
	void draw(Game* game, renderPass pass = renderPass::e_scene);
};

