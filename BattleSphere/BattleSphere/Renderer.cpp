#pragma once

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

////
//320x480
//320x568
//360x640
//480x800
//720x1280
//768x1024
//800x600
//1024x768
//1152x864
//1280x600
//1280x720
//1280x768
//1280x800
//1280x960
//1280x1024
//1360x768
//1366x768
//1400x1050
//1440x900
//1600x900
//1680x1050
//1920x1080
//1920x1200
//2160x3840
//2560x1440
//4320x7680

#include "GraphicResources.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "Lights.h"
#include "Bloom.h"
#include "Clock.h"
#include "Game.h"
#include "Camera.h"
#include "LightCulling.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "ShadowMapping.h"
#include "Transparency.h"
#include "Menu.h"
#include <system_error>

//TODO: REMOVE
float g_abc = 0;

using namespace DirectX;

GraphicResources g_graphicResources;
Bloom* g_bloom = nullptr;
Menu* g_menu = nullptr;

//TODO Remove
ConstantBuffer* g_constantBufferMaterials = nullptr;

ID3D11Buffer* g_vertexBufferFSQuad = nullptr;
ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;
VertexShader g_vertexShaderFinalRender;
PixelShader g_pixelShaderFinalRender;
PixelShader g_pixelShaderDownsample;
PixelShader g_pixelShaderBillboard;

Clock* g_Clock;
Game* g_Game;
LightCulling* g_lightCulling;

Transparency g_transparency;
ShadowMapping* g_shadowMapping;
GameState* g_gameState;
MainMenuState* g_mainMenuState;

int g_sunMoonIndex;

struct MaterialTest
{
	XMVECTOR Ka, Kd, Ks, Ke;
};
MaterialTest* g_materialTest = nullptr;

struct PosCol
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
};

PosCol vertexData[3]
{
	0.0f, 0.5f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,0.0f,

	-0.5f, -0.5f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.5f, -0.5f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,0.0f
};

void createFullscreenQuad()
{
	//Fullscreen quad
	PosCol fsQuad[6] =
	{
		-1.0f, 1.0f, 0.0f,	//v0 pos	L T
		0.0f, 0.0f,			//v0 tex
		0.0f, 0.0f, 0.0f,

		1.0, -1.0f, 0.0f,	//v1 pos	R B
		1.0f, 1.0f,			//v1 tex
		0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, 0.0f, //v2 pos	L B
		0.0f, 1.0f,			//v2 tex
		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 0.0f,	//v3 pos	L T
		0.0f, 0.0f,			//v3 tex
		0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,	//v4 pos	R T
		1.0f, 0.0f,			//v4 tex
		0.0f, 0.0f, 0.0f,

		1.0f, -1.0f, 0.0f,	//v5 pos	R B
		1.0f, 1.0f,			//v5 tex
		0.0f, 0.0f, 0.0f
	};

	D3D11_BUFFER_DESC bufferDescFSQuad;
	memset(&bufferDescFSQuad, 0, sizeof(bufferDescFSQuad));
	bufferDescFSQuad.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescFSQuad.Usage = D3D11_USAGE_DEFAULT;
	bufferDescFSQuad.ByteWidth = sizeof(fsQuad);

	D3D11_SUBRESOURCE_DATA dataFSQuad;
	dataFSQuad.pSysMem = fsQuad;

	// create a Vertex Buffer
	HRESULT result = DX::getInstance()->getDevice()->CreateBuffer(&bufferDescFSQuad, &dataFSQuad, &g_vertexBufferFSQuad);
	if (FAILED(result))
		MessageBox(NULL, L"gvertexBufferFSQuad", L"Error", MB_OK | MB_ICONERROR);

	g_vertexShaderFinalRender = VertexShader(L"VertexShaderFinalRender.hlsl");
	g_pixelShaderFinalRender = PixelShader(L"PixelShaderFinalRender.hlsl");
	g_pixelShaderDownsample = PixelShader(L"PixelShaderDownSample.hlsl");
	g_pixelShaderBillboard = PixelShader(L"PixelShaderBillboard.hlsl");
}

void setupTestTriangle()
{
	// VERTEX BUFFER
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;

	bufferDesc.ByteWidth = sizeof(vertexData);
	data.pSysMem = vertexData;

	HRESULT result = DX::getInstance()->getDevice()->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in testTriangle", L"Error", MB_OK | MB_ICONERROR);

	gVS = VertexShader(L"VertexShader.hlsl");
	gPS = PixelShader(L"PixelShader.hlsl");

	//Setup material
	g_materialTest = new MaterialTest();
	g_materialTest->Ka = XMVectorSet(1.0f, 0.0f, 0.0f, 2);
	g_materialTest->Kd = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	g_materialTest->Ks = XMVectorSet(1.0f, 1.0f, 1.0f, 100.0f);
	g_materialTest->Ke = XMVectorSet(0.5f, 0.5f, 0.5f, 100.0f);
	g_constantBufferMaterials = new ConstantBuffer(g_materialTest, sizeof(MaterialTest));
}

void createRenderResources()
{
	createFullscreenQuad();

	g_bloom = new Bloom();
	g_menu = new Menu();
	g_shadowMapping = new ShadowMapping();
	g_lightCulling = new LightCulling();
	g_lightCulling->initialize();
	g_lightCulling->computeFrustum();
	XMVECTOR camPos = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - g_shadowMapping->getCamera()->getPosition());
	Lights::getInstance()->addDirectionalLight(XMVectorGetX(camPos), XMVectorGetY(camPos), XMVectorGetZ(camPos),
		(float)238 / 255, (float)220 / 255, (float)165 / 255, 5.0f);
}

void setUserInterfacePipeline()
{
	//DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
	DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), NULL);
	DX::getInstance()->getDeviceContext()->OMSetBlendState(g_graphicResources.getBlendState(), NULL, 0xffffffff);

	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_menu->getCamera(false)->getConstantBufferVP()->getConstantBuffer());
	DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());

	DX::getInstance()->getDeviceContext()->VSSetShader(&g_menu->getVertexShader()->getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&g_menu->getPixelShader(2)->getPixelShader(), nullptr, 0);
}

void downsample()
{
	g_bloom->setRenderTarget(nullptr, renderPass::e_downSample);
	//DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(nullptr, 1);

	DX::getInstance()->getDeviceContext()->VSSetShader(&g_vertexShaderFinalRender.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&g_pixelShaderDownsample.getPixelShader(), nullptr, 0);

	UINT32 vertexSize = sizeof(PosCol);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &g_vertexBufferFSQuad, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_vertexShaderFinalRender.getvertexLayout());

	DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());
	g_bloom->setShaderResource(renderPass::e_downSample);
	g_graphicResources.bindDepthStencilState();

	g_graphicResources.setViewPortDim((UINT)(DX::getInstance()->getWidth() * 0.25f), (UINT)(DX::getInstance()->getHeight() * 0.25f));
	DX::getInstance()->getDeviceContext()->Draw(6, 0);
	g_graphicResources.setViewPortDim((UINT)DX::getInstance()->getWidth(), (UINT)DX::getInstance()->getHeight());

	ID3D11ShaderResourceView* nullSRV = { nullptr };
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);

}

void shadowRender()
{
	float clearColour[] = { 0, 0, 0, 0 };

	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), g_shadowMapping->getDepthStencilView());
	DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
	DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_shadowMapping->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_shadowMapping->getCamera()->getConstantBufferVP()->getConstantBuffer());

	DX::getInstance()->getDeviceContext()->VSSetShader(&g_shadowMapping->getVertexShader().getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(nullptr, nullptr, 0);

	UINT32 vertexSize = sizeof(PosCol);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_shadowMapping->getVertexShader().getvertexLayout());

	g_Game->draw(renderPass::e_shadow);
}

void billboardRender()
{
	DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&g_pixelShaderBillboard.getPixelShader(), nullptr, 0);
}

void transparencyRender()
{
	DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->OMSetBlendState(g_graphicResources.getBlendState(), NULL, 0xFFFFFFFF);
}

void finalRender()
{
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), NULL);

	DX::getInstance()->getDeviceContext()->VSSetShader(&g_vertexShaderFinalRender.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&g_pixelShaderFinalRender.getPixelShader(), nullptr, 0);

	UINT32 vertexSize = sizeof(PosCol);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &g_vertexBufferFSQuad, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_vertexShaderFinalRender.getvertexLayout());
	DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());
	g_bloom->setShaderResource(renderPass::e_final);

	DX::getInstance()->getDeviceContext()->Draw(6, 0);

	ID3D11ShaderResourceView* nullRTV = { NULL };
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &nullRTV);
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(1, 1, &nullRTV);
}

void setScreen()
{
	if (DX::getInstance()->fullScreenIsSet())
		DX::getInstance()->getSwapChain()->SetFullscreenState(true, nullptr);
	else
		DX::getInstance()->getSwapChain()->SetFullscreenState(false, nullptr);

	float width = DX::getInstance()->getWidth();
	float height = DX::getInstance()->getHeight();

	// Change camera aspect ratio
	float nearPlane = 0.1f;
	float farPlane = 500.0f;
	DX::getInstance()->reInitializeCam(width, height, nearPlane, farPlane);

	// Clear backbuffer views
	ID3D11RenderTargetView* nullViews = nullptr;
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, &nullViews, nullptr);
	if ((*g_graphicResources.getBackBuffer())) (*g_graphicResources.getBackBuffer())->Release();
	g_graphicResources.releaseDepthStencilViews();
	DX::getInstance()->getDeviceContext()->Flush();

	// Resize swapchain
	DX::getInstance()->getSwapChain()->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	// Update render target, depth stencil and viewport
	g_graphicResources.updateRenderTarget();

	// Update render target, depth stencil and viewport for shadow mapping, bloom and menu
	if (g_shadowMapping) delete g_shadowMapping;
	g_shadowMapping = new ShadowMapping();

	if (g_bloom) delete g_bloom;
	g_bloom = new Bloom();

	if (g_menu) delete g_menu;
	g_menu = new Menu();

	if (g_lightCulling) delete g_lightCulling;
	g_lightCulling = new LightCulling();
	g_lightCulling->initialize();
	g_lightCulling->computeFrustum();
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance); // Initialize resources and return window handler
	createRenderResources(); // Creates instances of graphics classes etc.
	//g_lightCulling->initialize();
	//g_lightCulling->computeFrustum();
	g_transparency.initialize();

	if (wndHandle)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); // (void)io;
		ImGui_ImplWin32_Init(wndHandle);
		ImGui_ImplDX11_Init(DX::getInstance()->getDevice(), DX::getInstance()->getDeviceContext());
		ImGui::StyleColorsLight();
		ImGui::GetIO().FontGlobalScale = 1.0;
		ShowWindow(wndHandle, nCmdShow);


		setupTestTriangle();


		g_Clock = new Clock();
		g_Game = new Game();
		g_gameState = new GameState(g_Game);
		g_mainMenuState = new MainMenuState();
		g_Game->pushState(g_gameState);
		g_Game->pushState(g_mainMenuState);
		g_Game->changeState(stateType::e_mainMenu); // Set initial state for the game
		//g_Game->changeState(stateType::e_gameState); // Set initial state for the game

		int counterFrames = 0;
		int fps = 0;
		float sunColor[4] = { (float)238 / 255, (float)247 / 255, (float)222 / 255, 0 };
		float moonColor[4] = { 0 };
		float rotation = 0;
		float intensity = 5.0f;
		XMVECTOR sunStartPos = g_shadowMapping->getCamera()->getPosition();
		XMVECTOR sunPos = g_shadowMapping->getCamera()->getPosition();
		XMVECTOR moonPos = XMVectorSet(0, -2, 0, 0);
		float speed = 1.0f;
		float rotCoeff = 0;

		/*int tempIndex = Lights::getInstance()->addAreaLight(0, 0, 0, 20, 1, 0, 1, 20);
		float positions[2];*/

		Graph::getInstance()->createVertexBuffer();

		DX::getInstance()->setScreen(false, 100, 200);

		while (WM_QUIT != msg.message)
		{
			if (DX::getInstance()->screenChanged())
				setScreen();

				g_Clock->calcDeltaTime();
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				if (msg.wParam == VK_ESCAPE)
					msg.message = WM_QUIT;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//// UPDATE ////
				if (g_Game->update(g_Clock->getDeltaTime() * 1.5f))
				{
					msg.message = WM_QUIT;
					DispatchMessage(&msg);
				}

				//// SET PIPELINE ////
				float clearColour[] = { 0, 0, 0, 0 };
				UINT32 vertexSize = sizeof(PosCol);
				UINT32 offset = 0;

				if (g_Game->isActive(stateType::e_gameState))
				{
					// SHADOW MAPPING
					ID3D11ShaderResourceView* nullSRV = nullptr;
					DX::getInstance()->getDeviceContext()->PSSetShaderResources(3, 1, &nullSRV);
					ID3D11RenderTargetView* nullRTV = nullptr;
					DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, &nullRTV, NULL);
					shadowRender();

					DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());

					g_lightCulling->updateSubresource();
					g_lightCulling->cullLights();

					DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);

					DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
					g_bloom->clearRenderTarget();
					// BLOOM
					g_bloom->setRenderTarget(g_graphicResources.getDepthStencilView(), renderPass::e_scene);

					g_Game->draw(renderPass::e_particles);
					DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, DX::getInstance()->getCam()->getConstantBufferVP()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->GSSetConstantBuffers(0, 1, DX::getInstance()->getCam()->getConstantBufferVP()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, DX::getInstance()->getCam()->getConstantBufferPosition()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, g_constantBufferMaterials->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(3, 1, g_shadowMapping->getCamera()->getConstantBufferVP()->getConstantBuffer());

					DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
					DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

					ID3D11ShaderResourceView* shadowSRV = g_shadowMapping->getShaderResourceView();
					DX::getInstance()->getDeviceContext()->PSSetShaderResources(3, 1, &shadowSRV);

					DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());

					DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
					DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					DX::getInstance()->getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());
				}
				else if (g_Game->isActive(stateType::e_mainMenu))
				{
					DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());

					DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
					DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
					DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), NULL);
					DX::getInstance()->getDeviceContext()->OMSetBlendState(g_graphicResources.getBlendState(), NULL, 0xffffffff);

					DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_menu->getCamera(false)->getConstantBufferVP()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());

					DX::getInstance()->getDeviceContext()->VSSetShader(&g_menu->getVertexShader()->getVertexShader(), nullptr, 0);
					DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->PSSetShader(&g_menu->getPixelShader(0)->getPixelShader(), nullptr, 0);


					DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_menu->getVertexShader()->getvertexLayout());
				}

				//// RENDER ////

				if (g_Game->isActive(stateType::e_gameState))
				{
					DX::getInstance()->getDeviceContext()->OMSetBlendState(nullptr, NULL, 0xFFFFFFFF);

					g_Game->draw(renderPass::e_opaque);

					billboardRender();
					g_Game->draw(renderPass::e_billboard);

					transparencyRender();
					g_Game->draw(renderPass::e_transparent);

					for (int i = 0; i < XUSER_MAX_COUNT; i++)
					{
						DX::getInstance()->getDeviceContext()->OMSetBlendState(g_graphicResources.getBlendState(), NULL, 0xFFFFFFFF);
						if (Graph::getInstance()->getActive(i))
						{
							Graph::getInstance()->updatePulse(i, g_Clock->getDeltaTime());
							Graph::getInstance()->draw(i);
						}
					}

					// Bloom
					downsample();
					g_bloom->run();

					finalRender();

					// User interface
					setUserInterfacePipeline();
					g_Game->draw(renderPass::e_userInterface);


					rotation += rotCoeff * speed * g_Clock->getDeltaTime();
					if (rotation >= 360)
						rotation -= 360;
					float rotInRad = XMConvertToRadians(rotation);

					sunPos = XMVector3Rotate(sunStartPos, XMVectorSet((float)sin(rotInRad / 2) * 0.5f, 0, (float)sin(rotInRad / 2), (float)cos(rotInRad / 2)));
					moonPos = XMVector3Rotate(sunPos, XMVectorSet((float)sin(XM_PI / 2) * 0.5f, 0, (float)sin(XM_PI / 2), (float)cos(XM_PI / 2)));
					XMVECTOR sunDir = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - sunPos);
					XMVECTOR moonDir = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - moonPos);
					float sunColorCoeff = (XMVectorGetX(XMVector3Dot(sunDir, XMVectorSet(0, -1, 0, 0))) > 0.0f) ? XMVectorGetX(XMVector3Dot(sunDir, XMVectorSet(0, -1, 0, 0))) : 0.0f;
					float moonColorCoeff = (XMVectorGetX(XMVector3Dot(moonDir, XMVectorSet(0, -1, 0, 0))) > 0.0f) ? XMVectorGetX(XMVector3Dot(moonDir, XMVectorSet(0, -1, 0, 0))) : 0.0f;

					sunColor[0] = 255 * pow(sunColorCoeff, 0.1f) / 255;
					sunColor[1] = 235 * pow(sunColorCoeff, 0.35f) / 255;
					sunColor[2] = 215 * pow(sunColorCoeff, 0.8f) / 255;

					moonColor[0] = 50 * pow(moonColorCoeff * 0.6f, 0.8f) / 255; //0.05f för lila
					moonColor[1] = 50 * pow(moonColorCoeff * 0.6f, 0.2f) / 255;	// 0.5f för lila
					moonColor[2] = 70 * pow(moonColorCoeff * 0.8f, 0.1f) / 255;

					if (sunColorCoeff > 0)
					{
						g_shadowMapping->getCamera()->setPosition(sunPos);
						rotCoeff = 1.4f - sunColorCoeff;

						Lights::getInstance()->setColor(g_sunMoonIndex, sunColor[0], sunColor[1], sunColor[2]);
						Lights::getInstance()->setDirection(g_sunMoonIndex, XMVectorGetX(sunDir), XMVectorGetY(sunDir), XMVectorGetZ(sunDir));
						Lights::getInstance()->setIntensity(g_sunMoonIndex, intensity);
					}
					else
					{
						g_shadowMapping->getCamera()->setPosition(moonPos);
						rotCoeff = 1.4f - moonColorCoeff;

						Lights::getInstance()->setColor(g_sunMoonIndex, moonColor[0], moonColor[1], moonColor[2]);
						Lights::getInstance()->setDirection(g_sunMoonIndex, XMVectorGetX(moonDir), XMVectorGetY(moonDir), XMVectorGetZ(moonDir));
						Lights::getInstance()->setIntensity(g_sunMoonIndex, intensity);
					}
					g_shadowMapping->getCamera()->updateBuffers();
				}
				else if (g_Game->isActive(stateType::e_mainMenu))
				{
					g_Game->draw(renderPass::e_menu);

					DX::getInstance()->getDeviceContext()->PSSetShader(&g_menu->getPixelShader(1)->getPixelShader(), nullptr, 0);
					g_Game->draw(renderPass::e_menuAni);

					DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
					g_lightCulling->updateSubresource();
					g_lightCulling->cullLights();
					//DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
					//DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
					DX::getInstance()->getDeviceContext()->OMSetBlendState(0, 0, 0xffffffff);
					DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), g_graphicResources.getDepthStencilView());
					DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_menu->getCamera(true)->getConstantBufferVP()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, g_menu->getCamera(true)->getConstantBufferPosition()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, g_constantBufferMaterials->getConstantBuffer());

					DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
					DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

					//DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
					DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					DX::getInstance()->getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());

					g_Game->draw(renderPass::e_menuScene);

					//finalRender();

					ID3D11ShaderResourceView* nullRTV = { NULL };
					DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &nullRTV);
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				ImGui::Begin("Settings");

				ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
				//ImGui::ColorPicker4("Pick a color", moonColor);
				//ImGui::SliderFloat("Rotation: ", &speed, 0, 10);
				//ImGui::SliderFloat("Intensity: ", &intensity, 0, 100);
				/*ImGui::SliderFloat("Pos X: ", &positions[0], -200, 200);
				ImGui::SliderFloat("Pos Z: ", &positions[1], -200, 200);
				Lights::getInstance()->setPosition(tempIndex, positions[0], 5, positions[1]);*/

				ImGui::End();
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				DX::getInstance()->getSwapChain()->Present(0, 0);

				counterFrames++;
				if (g_Clock->getTimeInSec() > 1.0)
				{
					fps = counterFrames;
					counterFrames = 0;
					g_Clock->resetSecTimer();

				}

				//realistic colours
				//sunColor[0] = 255 * pow(sunColorCoeff, 0.1f) / 255;
				//sunColor[1] = 235 * pow(sunColorCoeff, 0.35f) / 255;
				//sunColor[2] = 215 * pow(sunColorCoeff, 0.8f) / 255;
				//moonColor[0] = 50 * pow(moonColorCoeff * 0.6f, 0.8f) / 255; //0.05f för lila
				//moonColor[1] = 50 * pow(moonColorCoeff * 0.6f, 0.2f) / 255;	// 0.5f för lila
				//moonColor[2] = 70 * pow(moonColorCoeff * 0.8f, 0.1f) / 255;

				g_Clock->resetStartTimer();
			}
		}
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		//TODO Release
		_vertexBuffer->Release();
		g_vertexBufferFSQuad->Release();
		gVS.release();
		gPS.release();
		delete g_Clock;
		g_Game->release();
		delete g_Game;
		g_vertexShaderFinalRender.release();
		g_pixelShaderFinalRender.release();
		g_pixelShaderDownsample.release();
		g_pixelShaderBillboard.release();
		Graph::getInstance()->release();
		delete Graph::getInstance();

		//Remove
		delete g_bloom;
		delete g_materialTest;
		delete g_constantBufferMaterials;
		delete g_mainMenuState;
		delete g_gameState;
		delete g_menu;
		delete g_shadowMapping;
		delete g_lightCulling;
		Lights::releaseInstance();
		if (DX::getInstance())
			DX::getInstance()->release();
		delete DX::getInstance();
		DestroyWindow(wndHandle);
	}


	return (int)msg.wParam;
}

