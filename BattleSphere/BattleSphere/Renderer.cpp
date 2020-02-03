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
#include "Camera.h"
#include "Light.h"
#include "Bloom.h"

using namespace DirectX;

GraphicResources g_graphicResources;
Bloom* g_bloom = nullptr;

//TODO Remove
Camera* g_camera = nullptr;
Light* g_light = nullptr;
ConstantBuffer* g_constantBufferMaterials = nullptr;

ID3D11Buffer* g_vertexBufferFSQuad = nullptr;
ID3D11Buffer* _vertexBuffer = nullptr;
VertexShader gVS;
PixelShader gPS;
VertexShader g_vertexShaderFinalRender;
PixelShader g_pixelShaderFinalRender;

struct MaterialTest
{
	XMVECTOR Ka, Kd, Ks, Ke;
};
MaterialTest* g_materialTest = nullptr;

struct PosCol
{
	float x, y, z;
	float r, g, b;
};

struct PosUV
{
	float x, y, z;
	float u, v, a;
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

void createFullscreenQuad()
{
	//Fullscreen quad
	PosUV fsQuad[6] =
	{
		-1.0f, 1.0f, 0.0f,	//v0 pos	L T
		0.0f, 0.0f,	0.0f,	//v0 tex

		-1.0f, -1.0f, 0.0f, //v2 pos	L B
		0.0f, 1.0f,	0.0f,	//v2 tex

		1.0, -1.0f, 0.0f,	//v1 pos	R B
		1.0f, 1.0f,	0.0f,	//v1 tex


		-1.0f, 1.0f, 0.0f,	//v3 pos	L T
		0.0f, 0.0f,	0.0f,	//v3 tex

		1.0f, -1.0f, 0.0f,	//v5 pos	R B
		1.0f, 1.0f, 0.0f,	//v5 tex

		1.0f, 1.0f, 0.0f,	//v4 pos	R T
		1.0f, 0.0f,	0.0f	//v4 tex
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

	// TODO: Move camera and light to game?
	g_camera = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 0.1f, 200.0f);
	//g_light = new Light(XMVectorSet(-2.0f, 5.0f, -5.0f, 1.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	g_light = new Light(XMVectorSet(1.0f, 0.0f, -5.0f, 1.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));

	g_bloom = new Bloom();
}

void finalRender()
{
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, g_graphicResources.getBackBuffer(), NULL);

	DX::getInstance()->getDeviceContext()->VSSetShader(&g_vertexShaderFinalRender.getVertexShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	DX::getInstance()->getDeviceContext()->PSSetShader(&g_pixelShaderFinalRender.getPixelShader(), nullptr, 0);

	UINT32 vertexSize = sizeof(PosUV);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &g_vertexBufferFSQuad, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->IASetInputLayout(&g_vertexShaderFinalRender.getvertexLayout());

	DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, g_graphicResources.getSamplerState());
	g_bloom->setShaderResource();
	
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

	createRenderResources(); // Creates instances of graphics classes etc.

	if (wndHandle)
	{
		ShowWindow(wndHandle, nCmdShow);

		setupTestTriangle();

		///////////////
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				if (msg.wParam == VK_ESCAPE)
					msg.message = WM_QUIT;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//// RENDER ////

				DX::getInstance()->getDeviceContext()->RSSetState(g_graphicResources.getRasterizerState());
				float clearColour[] = { 0, 0, 0, 1 };

				DX::getInstance()->getDeviceContext()->ClearRenderTargetView(*g_graphicResources.getBackBuffer(), clearColour);
				g_bloom->clearRenderTarget();

				// BLOOM
				g_bloom->setRenderTarget();

				DX::getInstance()->getDeviceContext()->VSSetConstantBuffers(0, 1, g_camera->getConstantBufferVP()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(0, 1, g_light->getConstantuffer()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(1, 1, g_camera->getConstantBufferPosition()->getConstantBuffer());
				DX::getInstance()->getDeviceContext()->PSSetConstantBuffers(2, 1, g_constantBufferMaterials->getConstantBuffer());
				
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

				g_bloom->run();

				finalRender();

				DX::getInstance()->getSwapChain()->Present(0, 0);

			}
		}

		//TODO Release
		_vertexBuffer->Release();
		g_vertexBufferFSQuad->Release();
		gVS.release();
		gPS.release();
		g_vertexShaderFinalRender.release();
		g_pixelShaderFinalRender.release();
		DX::getInstance()->release();
		delete DX::getInstance();

		//Remove
		delete g_camera;
		delete g_light;
		delete g_bloom;
		delete g_materialTest;
		delete g_constantBufferMaterials;

		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

