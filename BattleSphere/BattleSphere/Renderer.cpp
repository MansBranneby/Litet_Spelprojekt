#pragma once

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

////
#include "GraphicResources.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "Clock.h"
#include "Camera.h"
#include "Light.h"


using namespace DirectX;

GraphicResources g_graphicResources;


//TODO Remove
Camera* g_camera = nullptr;
Light* g_light = nullptr;

ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;

Clock* gClock;

struct PosCol
{
	float x, y, z;
	float r, g, b;
};

PosCol vertexData[3]
{
	0.0f, 0.5f, 0.0f,
	1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.0f,
	0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, 0.0f,
	0.0f, 1.0f, 0.0f
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
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#define _ITERATOR_DEBUG_LEVEL 0
	#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance); // Initialize resources and return window handler

	// TODO: Move camera and light to game?
	g_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.1f, 200.0f);
	g_light = new Light(XMVectorSet(-2.0f, 5.0f, -5.0f, 1.0f), XMVectorSet(0.8f, 0.1f, 0.1f, 1.0f));

	if (wndHandle)
	{
		ShowWindow(wndHandle, nCmdShow);

		setupTestTriangle();

		gClock = new Clock();
		
		int counterFrames = 0;
		int fps = 0;

		///////////////
		while (WM_QUIT != msg.message)
		{
			gClock->calcDeltaTime();
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				if (msg.wParam == VK_ESCAPE)
					msg.message = WM_QUIT;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//TODO Ersätt med riktig render funktion eller i separata klasser
				//RENDER
				DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
				float clearColour[] = { 0, 0, 0, 1 };

				DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
				DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), NULL);

				DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_camera->getConstantBufferVP()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, g_light->getConstantuffer()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, g_camera->getConstantBufferPosition()->getConstantBuffer());

				DX::getInstance()->getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
				DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
				DX::getInstance()->getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

				UINT32 vertexSize = sizeof(float) * 6;
				UINT32 offset = 0;

				DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
				DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX::getInstance()->getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());

				DX::getInstance()->getDeviceContext()->Draw(3, 0);

				DX::getInstance()->getSwapChain()->Present(0, 0);

				counterFrames++;
				if (gClock->getTimeInSec() > 1.0)
				{
					fps = counterFrames;
					counterFrames = 0;
					gClock->resetSecTimer();
					OutputDebugStringA(std::to_string(fps).c_str());
					OutputDebugStringA("\n");
				}

				//std::string xd = std::to_string(gClock->getDeltaTime());
				//OutputDebugStringA(xd.c_str());
				gClock->resetStartTimer();
			}
		}

		//TODO Release
		_vertexBuffer->Release();
		gVS.release();
		gPS.release();
		DX::getInstance()->release();
		delete DX::getInstance();
		delete g_camera;
		delete g_light;

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

