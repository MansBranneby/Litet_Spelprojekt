#include "State.h"

State::State()
{
	m_paused = true;
	m_type = stateType::e_default;
}

State::~State()
{
}

void State::activateState(Game* game, stateType state)
{
	game->changeState(state);
}

stateType State::getType()
{
	return m_type;
}

void State::setPaused(bool paused)
{
	m_paused = paused;
}

bool State::isPaused()
{
	return m_paused;
}
