#pragma once
#include <d3d11.h>
#include "Camera.h"
//#include <d3dcompiler.h>
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")

class Camera; // Forward declaration

class DX 
{
private:
	DX();
	static DX* m_instance;

	// DirectX
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;

	// Data
	float m_width = 1920.0f;
	float m_height = 1080.0f;

	// Camera
	Camera* m_camera;

public:
	static DX* getInstance();

	Camera* getCam();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	IDXGISwapChain* getSwapChain();

	float getWidth();
	float getHeight();

	HRESULT createDirect3DContext(HWND wndHandle);
	void initializeCam(float width, float height, float nearPlane, float farPlane);

	void release();
};

