#include "Menu.h"

void Menu::initializeResources()
{
}

Menu::Menu()
{
	m_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.01f, 200.0f, false);
	m_cameraPerspective = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.01f, 200.0f, true);
	m_cameraPerspective->setCameraPosition(0.0f, 0.0f, -10.0f);
	//m_depthState = nullptr;
	m_vertexShader = new VertexShader(L"VertexShaderMenu.hlsl");
	m_pixelShader = new PixelShader(L"PixelShaderMenu.hlsl");
	m_pixelShaderAni = new PixelShader(L"PixelShaderUIAni.hlsl");
	m_pixelShaderUI = new PixelShader(L"PixelShaderUserInterface.hlsl");

	initializeResources();
}

Menu::~Menu()
{
	delete m_camera;
	delete m_cameraPerspective;
	
	m_vertexShader->release();
	m_pixelShader->release();
	m_pixelShaderAni->release();
	m_pixelShaderUI->release();
//	m_depthState->Release();

	delete m_vertexShader;
	delete m_pixelShader;
	delete m_pixelShaderAni;
	delete m_pixelShaderUI;
}

//ID3D11DepthStencilState* Menu::getDepthStencilState()
//{
//	//return m_depthState;
//}

Camera* Menu::getCamera(bool perspective)
{
	if (perspective)
		return m_cameraPerspective;
	else
		return m_camera;
}

VertexShader* Menu::getVertexShader()
{
	return m_vertexShader;
}

PixelShader* Menu::getPixelShader(int index)
{
	if (index == 0)
		return m_pixelShader;
	else if (index == 1)
		return m_pixelShaderAni;
	else
		return m_pixelShaderUI;
}