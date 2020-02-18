#include "Gamepad.h"

Gamepad::Gamepad()
{
	m_deadZoneX = 0.1f;
	m_deadZoneY = 0.1f;
	m_state.Gamepad.wButtons = 0;
	m_vibration.wLeftMotorSpeed = 0;
	m_vibration.wRightMotorSpeed = 0;
	m_time = 0.0f;
	m_vibActive = false;

	m_controllerId = -1;
	m_thumbLX = 0;
	m_thumbLY = 0;
	m_thumbRX = 0;
	m_thumbRY = 0;
	m_triggerL = 0;
	m_triggerR = 0;
}

void Gamepad::setId(int id)
{
	m_controllerId = id;
}

void Gamepad::setDeadZone(float x, float y)
{
	m_deadZoneX = x;
	m_deadZoneY = y;
}

XINPUT_GAMEPAD* Gamepad::getState()
{
	return &m_state.Gamepad;
}

bool Gamepad::refresh(float dt)
{
	if (m_controllerId == -1)
		return false;

	if (m_controllerId != -1)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_controllerId, &m_state) != ERROR_SUCCESS)
		{
			m_controllerId = -1;
			return false;
		}
		ZeroMemory(&m_vibration, sizeof(XINPUT_VIBRATION));
		if (m_vibActive)
		{
			m_time += dt;
			if (m_time > 0.1f)
				setVibration(0.0f);
		}

		float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

		m_thumbLX = (abs(normLX) < m_deadZoneX ? 0 : (abs(normLX) - m_deadZoneX) * (normLX / abs(normLX)));
		m_thumbLY = (abs(normLY) < m_deadZoneY ? 0 : (abs(normLY) - m_deadZoneY) * (normLY / abs(normLY)));

		if (m_deadZoneX > 0) m_thumbLX *= 1 / (1 - m_deadZoneX);
		if (m_deadZoneY > 0) m_thumbLY *= 1 / (1 - m_deadZoneY);

		float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

		m_thumbRX = (abs(normRX) < m_deadZoneX ? 0 : (abs(normRX) - m_deadZoneX) * (normRX / abs(normRX)));
		m_thumbRY = (abs(normRY) < m_deadZoneY ? 0 : (abs(normRY) - m_deadZoneY) * (normRY / abs(normRY)));

		if (m_deadZoneX > 0) m_thumbRX *= 1 / (1 - m_deadZoneX);
		if (m_deadZoneY > 0) m_thumbRY *= 1 / (1 - m_deadZoneY);

		m_triggerL = (float)m_state.Gamepad.bLeftTrigger / 255;
		m_triggerR = (float)m_state.Gamepad.bRightTrigger / 255;

		return true;
	}
	return false;
}

bool Gamepad::isPressed(WORD button)
{
	return (m_state.Gamepad.wButtons & button) != 0;
}

void Gamepad::setVibration(float speed)
{
	if (speed == 0.0f)
		m_vibActive = false;
	else
		m_vibActive = true;
	m_time = 0.0f;
	m_vibration.wLeftMotorSpeed = (int)floorf(65535 * speed);
	m_vibration.wRightMotorSpeed = (int)floorf(65535 * speed);
	XInputSetState(m_controllerId, &m_vibration);
}

float Gamepad::getThumbLX()
{
	return m_thumbLX;
}

float Gamepad::getThumbLY()
{
	return m_thumbLY;
}

float Gamepad::getThumbRX()
{
	return m_thumbRX;
}

float Gamepad::getThumbRY()
{
	return m_thumbRY;
}

float Gamepad::getTriggerL()
{
	return m_triggerL;
}

float Gamepad::getTriggerR()
{
	return m_triggerR;
}
