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

	int getControllerState();
public:
	Input();

	bool refresh(int user);
	bool isPressed(int user, WORD button);

	float getThumbLX(int user);
	float getThumbLY(int user);
	float getThumbRX(int user);
	float getThumbRY(int user);
	float getTriggerL(int user);
	float getTriggerR(int user);
};