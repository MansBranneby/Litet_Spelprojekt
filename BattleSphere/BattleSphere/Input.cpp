#include "Input.h"

Input::Input()
{
    getControllerState();
}

bool Input::refresh(int user)
{
    return m_gamepads[user].refresh();
}

bool Input::isPressed(int user, WORD button)
{
    return m_gamepads[user].isPressed(button);
}

float Input::getThumbLX(int user)
{
    return m_gamepads[user].getThumbLX();
}

float Input::getThumbLY(int user)
{
    return m_gamepads[user].getThumbLY();
}

float Input::getThumbRX(int user)
{
    return m_gamepads[user].getThumbRX();
}

float Input::getThumbRY(int user)
{
    return m_gamepads[user].getThumbRY();
}

float Input::getTriggerL(int user)
{
    return m_gamepads[user].getTriggerL();
}

float Input::getTriggerR(int user)
{
    return m_gamepads[user].getTriggerR();
}

int Input::getControllerState()
{
    int counter = 0;

    DWORD dwResult;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            // Controller is connected
            counter++;
            m_gamepads[i].setId(i);
        }
        else
        {
            // Controller is not connected 
        }
    }

	return counter;
}
