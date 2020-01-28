#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Xinput.h>
#include <cmath>
#include "DX.h"

#pragma comment(lib, "XInput.lib")

using namespace DirectX;

class Gamepad
{
private:
	int m_controllerId;
	XINPUT_STATE m_state;

	float m_deadZoneX;
	float m_deadZoneY;

	float m_thumbLX;
	float m_thumbLY;
	float m_thumbRX;
	float m_thumbRY;
	float m_triggerL;
	float m_triggerR;

public:
	Gamepad();

	void setId(int id);
	void setDeadZone(float x, float y);
	XINPUT_GAMEPAD* getState();
	bool refresh();
	bool isPressed(WORD button);

	float getThumbLX();
	float getThumbLY();
	float getThumbRX();
	float getThumbRY();
	float getTriggerL();
	float getTriggerR();
};