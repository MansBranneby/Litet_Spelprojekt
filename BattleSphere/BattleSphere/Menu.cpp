#include "Menu.h"

void Menu::initializeResources()
{
	HRESULT hr = CoInitialize(NULL);
	hr = CreateWICTextureFromFile(DX::getInstance()->getDevice(), L"Textures\\loadingScreen.png", NULL, &m_loadingScreenSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"Error in m_loadingScreenSRV", L"Error", MB_OK | MB_ICONERROR);
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
	m_pixelShaderLoadingScreen = new PixelShader(L"PixelShaderLoadingScreen.hlsl");

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
	m_pixelShaderLoadingScreen->release();
	
	m_loadingScreenSRV->Release();

	delete m_vertexShader;
	delete m_pixelShader;
	delete m_pixelShaderAni;
	delete m_pixelShaderUI;
	delete m_pixelShaderLoadingScreen;
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
	else if (index == 2)
		return m_pixelShaderUI;
	else
		return m_pixelShaderLoadingScreen;
}

void Menu::setLoadingScreenSRV()
{
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &m_loadingScreenSRV);
}
