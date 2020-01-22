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

using namespace DirectX;

GraphicResources g_graphicResources;


//TODO Remove
ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;

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

	HRESULT result = DX::getDevice()->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in testTriangle", L"Error", MB_OK | MB_ICONERROR);

	gVS = VertexShader(L"VertexShader.hlsl");
	gPS = PixelShader(L"PixelShader.hlsl");
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) 
{
	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance);; // Initialize resources and return window handler

	if (wndHandle)
	{
		ShowWindow(wndHandle, nCmdShow);

		setupTestTriangle();

		///////////////
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//TODO Ersätt med riktig render funktion eller i separata klasser
				//RENDER
				DX::getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
				float clearColour[3] = {0.0f, 0.0f, 0.0f};

				DX::getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
				DX::getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), g_graphicResources.getDepthStencilView());

				DX::getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
				DX::getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
				DX::getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
				DX::getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
				DX::getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

				UINT32 vertexSize = sizeof(float) * 6;
				UINT32 offset = 0;

				DX::getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
				DX::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				DX::getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());

				DX::getDeviceContext()->Draw(3, 0);
			}
		}

		//TODO Release
		DX::getDevice()->Release();
		DX::getDeviceContext()->Release();
		DX::getSwapChain()->Release();

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}