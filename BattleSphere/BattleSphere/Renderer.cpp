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

#include "Clock.h"
#include "Game.h"
#include "Camera.h"
#include "Light.h"
#include "LightCulling.h"



using namespace DirectX;

// TODO test quadtree
#include "QuadtreeNode.h"
QuadtreeNode* g_root;
BoundingVolume* g_BV;


GraphicResources g_graphicResources;

//TODO Remove
Camera* g_camera = nullptr;
ConstantBuffer* g_constantBufferMaterials = nullptr;

ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;

Clock* g_Clock;
Game* g_Game;
LightCulling g_lightCulling;


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

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#define _ITERATOR_DEBUG_LEVEL 0
	#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance); // Initialize resources and return window handler
	g_lightCulling.initialize();

	// TODO test quadtree
	DirectX::XMFLOAT3 levelPos = { 0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT2 halfWD = { 10.0f, 10.0f };
	DirectX::XMMATRIX rotMax = DirectX::XMMatrixRotationY(DirectX::XM_PI * 0.25f);
	//DirectX::XMMATRIX rotMax = DirectX::XMMatrixIdentity();
	std::vector<BoundingVolume*> BVs = { new OBB({-5.0f, 0.0f, -5.0f}, {1.0f, 1.0f}, DirectX::XMMatrixIdentity()), new OBB({-1.01f, 0.0f, -1.01f}, {1.0f, 1.0f}, DirectX::XMMatrixIdentity()) };
	g_root = new QuadtreeNode(levelPos, halfWD, BVs, 2, 0);


	// TODO: Move camera and light to game?
	g_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.1f, 200.0f);

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

				//TODO Ers�tt med riktig render funktion eller i separata klasser
				//RENDER
				DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
				float clearColour[] = { 0, 0, 0, 1 };
				g_lightCulling.cullLights();
				DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
				DX::getInstance()->getDeviceContext()->ClearDepthStencilView(g_graphicResources.getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
				DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), g_graphicResources.getDepthStencilView());

				DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_camera->getConstantBufferVP()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, g_camera->getConstantBufferPosition()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, g_constantBufferMaterials->getConstantBuffer());

				DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
				DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

				UINT32 vertexSize = sizeof(PosCol);
				UINT32 offset = 0;

				DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
				DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX::getInstance()->getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());

			

				returnInfo a = g_Game->update(g_Clock->getDeltaTime());
				g_lightCulling.setPosition(0, a.x, a.y, a.z);
				g_Game->draw();
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
					g_Game->updateSec();
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
		gVS.release();
		gPS.release();
		delete g_Clock;
		g_Game->release();
		delete g_Game;
		DX::getInstance()->release();
		delete DX::getInstance();

		//Remove
		delete g_camera;
		delete g_materialTest;
		delete g_constantBufferMaterials;

		// TODO delete dynamically allocated stuff
		delete g_root;

		for (int i = 0; i < BVs.size(); ++i)
			delete BVs[i];

		DestroyWindow(wndHandle);
	}


	return (int)msg.wParam;
}

