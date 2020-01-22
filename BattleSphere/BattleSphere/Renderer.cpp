#pragma once

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

////
#include "GraphicResources.h"

GraphicResources g_graphicResources;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) 
{
	MSG msg = { 0 };
	HWND wndHandle = g_graphicResources.initializeResources(hInstance);; // Initialize resources and return window handler
	/*
	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle); // Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); // And rasterizer state

		createMeshes(); // test
		setupTextures();

		createRenderTargets();
		createShaders();
		createShadersSP();
		createShadersShadowMap();

		createTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		samplerSetUp();
		createConstantBuffer();

		ShowWindow(wndHandle, nCmdShow);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); // (void)io;
		ImGui_ImplWin32_Init(wndHandle);
		ImGui_ImplDX11_Init(gDevice, gDeviceContext);
		ImGui::StyleColorsDark();

		std::unique_ptr<Keyboard> keyboard;
		std::unique_ptr<Mouse> mouse;
		Mouse::ButtonStateTracker tracker;

		keyboard = std::make_unique<Keyboard>();
		mouse = std::make_unique<Mouse>();
		mouse->SetWindow(wndHandle);
		POINT cursorPos;
		XMFLOAT3 velocity{ 0.0f, 0.0f, 0.0f };

		float pitch = 0.0f;
		float yaw = 0.0f;
		float lastT = -1.0f;
		LARGE_INTEGER clockFreq;
		LARGE_INTEGER startTime;
		LARGE_INTEGER delta;
		LARGE_INTEGER currTime;
		QueryPerformanceFrequency(&clockFreq);
		QueryPerformanceCounter(&startTime);

		///////////////
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				switch (msg.message)
				{
				case WM_KEYDOWN:
					Keyboard::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_KEYUP:
					Keyboard::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_MOUSEMOVE:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_RBUTTONDOWN:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_RBUTTONUP:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
				case WM_INPUT:
					Mouse::ProcessMessage(msg.message, msg.wParam, msg.lParam);
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Camera updates
				//
				//
				QueryPerformanceCounter(&currTime);
				delta.QuadPart = currTime.QuadPart - startTime.QuadPart;
				float deltaSeconds = (float)delta.QuadPart / clockFreq.QuadPart;
				startTime = currTime;

				DirectX::Mouse::State ms = mouse->GetState();
				DirectX::Keyboard::State kb = keyboard->GetState();


				mouse->SetMode(ms.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
				if (ms.positionMode == Mouse::MODE_RELATIVE)
				{

					yaw += XMConvertToRadians(ms.x);
					pitch += XMConvertToRadians(ms.y);
					pitch = min(XM_PI / 2, max(-XM_PI / 2, pitch));
					ms.x = 0;
					ms.y = 0;
				}

				XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
				XMMATRIX rotationYPos = XMMatrixRotationRollPitchYaw(0.0f, yaw, 0.0f);

				velocity.x = 0;
				velocity.y = 0;
				velocity.z = 0;

				if (kb.W)
					velocity.z += gCamera.distance * deltaSeconds;
				if (kb.S)
					velocity.z -= gCamera.distance * deltaSeconds;
				if (kb.A)
					velocity.x -= gCamera.distance * deltaSeconds;
				if (kb.D)
					velocity.x += gCamera.distance * deltaSeconds;
				if (kb.Space)
					velocity.y += gCamera.distance * deltaSeconds;
				if (kb.LeftControl)
					velocity.y -= gCamera.distance * deltaSeconds;
				if (kb.Home)
					velocity = { 0.0f, 0.0f, -2.0f };
				if (kb.Escape)
					msg.message = WM_QUIT;

				transform(velocity, rotation, rotationYPos);

				GetCursorPos(&cursorPos); // gets current cursor coordinates
				ScreenToClient(wndHandle, &cursorPos); // sets cursor coordinates relative to the program window. upper left corner of the screen = (0,0)
				float tempT = mousePicking(cursorPos);
				lastT = tempT;

				//
				//
				// RENDER //
				gDeviceContext->RSSetState(gRasterizerState);
				gClearColour[3] = 1.0;
				renderShadowMap();

				gDeviceContext->OMSetRenderTargets(3, gRenderTargetsDeferred, gDSV);
				gDeviceContext->ClearDepthStencilView(gDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[0], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[1], gClearColour);
				gDeviceContext->ClearRenderTargetView(gRenderTargetsDeferred[2], gClearColour);

				renderFirstPass();
				renderNormalMap();
				renderBoundingVolume();
				renderBillboard();

				gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);
				gDeviceContext->ClearRenderTargetView(gBackbufferRTV, gClearColour);

				renderSecondPass();

				update(lastT, cursorPos);

				gSwapChain->Present(0, 0);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		gVertexBufferFSQuad->Release();
		gConstantBuffer->Release();
		gShaderResourceDeferred[0]->Release();
		gShaderResourceDeferred[1]->Release();
		gShaderResourceDeferred[2]->Release();
		gRenderTargetsDeferred[0]->Release();
		gRenderTargetsDeferred[1]->Release();
		gRenderTargetsDeferred[2]->Release();
		gSamplerState->Release();

		gVertexLayout->Release();
		gVertexLayoutFSQuad->Release();

		gVertexShader->Release();
		gVertexShaderSP->Release();
		gGeometryShader->Release();
		gPixelShader->Release();
		gPixelShaderSP->Release();
		gPixelShaderBillboard->Release();
		gGeometryShaderBillboard->Release();

		gDSV->Release();
		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		delete gPillar;
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}*/
}