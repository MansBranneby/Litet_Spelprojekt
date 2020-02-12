#include "Menu.h"

void Menu::initializeResources()
{
}

Menu::Menu()
{
	m_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.01f, 200.0f, false);
	m_depthState = nullptr;

	initializeResources();
}

Menu::~Menu()
{
	delete m_camera;
	m_depthState->Release();
}

ID3D11DepthStencilState* Menu::getDepthStencilState()
{
	return m_depthState;
}

Camera* Menu::getCamera()
{
	return m_camera;
}
