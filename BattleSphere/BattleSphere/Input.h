#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "DX.h"
#include "Gamepad.h"

using namespace DirectX;

class Input
{
private:
	Gamepad m_gamepads[XUSER_MAX_COUNT];
	int m_gamepadIds[XUSER_MAX_COUNT];
	int m_nrOfGamepads;
	bool m_inputBlocked[XUSER_MAX_COUNT];

	int getControllerState();
public:
	Input();

	int getId(int user);
	int getNrOfGamepads();

	bool isBlocked(int id);
	void setBlocked(int id, bool inputBlocked);

	bool reconnectController(int user);
	bool refresh(int user);

	bool isPressed(int user, WORD button);

	float getThumbLX(int user);
	float getThumbLY(int user);
	float getThumbRX(int user);
	float getThumbRY(int user);
	float getTriggerL(int user);
	float getTriggerR(int user);
};