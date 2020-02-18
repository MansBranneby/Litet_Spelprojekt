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
#include "GraphicResources.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Camera.h"
#include "Light.h"
#include "Bloom.h"
#include "Clock.h"
#include "Game.h"
#include "Camera.h"
#include "Light.h"
#include "LightCulling.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "Menu.h"

using namespace DirectX;

GraphicResources g_graphicResources;
Bloom* g_bloom = nullptr;
Menu* g_menu = nullptr;

//TODO Remove
Camera* g_camera = nullptr;
ConstantBuffer* g_constantBufferMaterials = nullptr;

ID3D11Buffer* g_vertexBufferFSQuad = nullptr;
ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;
VertexShader g_vertexShaderFinalRender;
PixelShader g_pixelShaderFinalRender;
PixelShader g_pixelShaderDownsample;

Clock* g_Clock;
Game* g_Game;
LightCulling g_lightCulling;
GameState* g_gameState;
MainMenuState* g_mainMenuState;

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

	g_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.1f, 200.0f);
	g_bloom = new Bloom();
	g_menu = new Menu();

	g_gameState = new GameState();
	g_mainMenuState = new MainMenuState();
}

void downsample()
{
	g_bloom->setRenderTarget(g_graphicResources.getDepthStencilView(), renderPass::e_downSample);

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

	g_graphicResources.setViewPortDim((UINT)(DX::getInstance()->getWidth() * 0.25f), (UINT)(DX::getInstance()->getHeight() * 0.25f));
	DX::getInstance()->getDeviceContext()->Draw(6, 0);
	g_graphicResources.setViewPortDim((UINT)DX::getInstance()->getWidth(), (UINT)DX::getInstance()->getHeight());

	float clearColour[] = { 0, 0, 0, 1 };
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

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#define _ITERATOR_DEBUG_LEVEL 0
	#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance); // Initialize resources and return window handler
	g_lightCulling.initialize();
	createRenderResources(); // Creates instances of graphics classes etc.

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

		int index = g_lightCulling.addPointLight(-10, 25, 0, 55, 1, 0.5f, 0.125f, 1);
		g_lightCulling.setColor(index, float(255) / 255, float(0) / 255, float(97) / 255);
		index = g_lightCulling.addDirectionalLight(-0.5f, -1, 0.5f, 1, 1, 1, 0.6f);
		g_lightCulling.setColor(index, float(19) / 255, float(62) / 255, float(124) / 255);
		index = g_lightCulling.addDirectionalLight(0.5f, -0.1f, -0.5f, 0.1f, 0.2f, 0.6f, 0.8f);
		g_lightCulling.setColor(index, float(19) / 255, float(62) / 255, float(124) / 255);
		index = g_lightCulling.addSpotLight(-35, 30, -5, 50, -0.3f, -1, 0.3f, 1.0f, 0.9f, 0.9f, 25, 1);
		g_lightCulling.setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);
		index = g_lightCulling.addSpotLight(0, 5, -45, 50, 0, -0.5f, 1.0f, 1.0f, 0.9f, 0.9f, 25, 1);
		g_lightCulling.setColor(index, float(234) / 255, float(185) / 255, float(217) / 255);
		
		g_Clock = new Clock();
		g_Game = new Game();
		g_Game->pushState(g_gameState);
		g_Game->pushState(g_mainMenuState);
		g_Game->changeState(stateType::e_mainMenu); // Set initial state for the game

		int counterFrames = 0;
		int fps = 0;
		/*For IMGUI*/
		float lightColor[3] = { 0 };
		float lightIntensity = 1;
		float lightRange = 1;
		

		///////////////
		while (WM_QUIT != msg.message)
		{
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

				g_Game->update(g_Clock->getDeltaTime());

				//// SET PIPELINE ////
				float clearColour[] = { 0, 0, 0, 1 };
				UINT32 vertexSize = sizeof(PosCol);
				UINT32 offset = 0;
				
				if (g_Game->isActive(stateType::e_gameState))
				{
					DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
					g_lightCulling.cullLights();
					DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
					DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
					DX::getInstance()->getDeviceContext()->OMSetBlendState(0, 0, 0xffffffff);

					g_bloom->clearRenderTarget();

					// BLOOM
					g_bloom->setRenderTarget(g_graphicResources.getDepthStencilView(), renderPass::e_scene);

					DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_camera->getConstantBufferVP()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, g_camera->getConstantBufferPosition()->getConstantBuffer());
					DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, g_constantBufferMaterials->getConstantBuffer());

					DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
					DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
					DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

					//DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
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
					DX::getInstance()->getDeviceContext()->PSSetShader(&g_menu->getPixelShader()->getPixelShader(), nullptr, 0);


					DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_menu->getVertexShader()->getvertexLayout());
				}
			
				//// RENDER ////
				
				if (g_Game->isActive(stateType::e_gameState))
				{
					g_Game->draw(0);
					downsample();

					g_bloom->run();

					finalRender();
				}
				else if (g_Game->isActive(stateType::e_mainMenu))
				{
					g_Game->draw(0);

					DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
					g_lightCulling.cullLights();
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

					g_Game->draw(1);

					//finalRender();

					ID3D11ShaderResourceView* nullRTV = { NULL };
					DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &nullRTV);
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				ImGui::Begin("Settings");
				
				ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
				
				ImGui::ColorPicker3("Select color", lightColor);
				ImGui::SliderFloat("Select Range", &lightRange, 0, 100);
				ImGui::SliderFloat("Select Intensity", &lightIntensity, 0, 10);
				g_lightCulling.setColor(0, lightColor[0], lightColor[1], lightColor[2]);
				g_lightCulling.setRange(0, lightRange);
				g_lightCulling.setIntensity(0, lightIntensity);
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
					//g_Game->updateSec();
					// TODO: delet dis (visa fps)
					/*OutputDebugStringA(std::to_string(fps).c_str());
					OutputDebugStringA("\n");*/
				
				}
				

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
		DX::getInstance()->release();
		delete DX::getInstance();

		//Remove
		delete g_camera;
		delete g_bloom;
		delete g_materialTest;
		delete g_constantBufferMaterials;
		delete g_mainMenuState;
		delete g_gameState;
		delete g_menu;

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

