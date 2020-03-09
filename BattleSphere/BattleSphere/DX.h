#pragma once
#include <d3d11.h>
#include <dxgidebug.h>
#include "Camera.h"
//#include <d3dcompiler.h>
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")

class Camera; // Forward declaration
class Particles;

class DX 
{
private:
	DX();
	static DX* m_instance;

	// DirectX
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11Debug* m_debug;
	ID3D11DepthStencilState* m_pDSStateEnabled;
	ID3D11DepthStencilState* m_pDSStateDisabled;

	void createStencilStates();
	// Data
	float m_width = 1920.0f;
	float m_height = 1080.0f;

	// Camera
	Camera* m_camera;

	// Particles
	Particles* m_particles;

public:
	static DX* getInstance();

	Camera* getCam();
	Particles* getParticles();
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	IDXGISwapChain* getSwapChain();

	ID3D11DepthStencilState* getDSSEnabled();
	ID3D11DepthStencilState* getDSSDisabled();
	float getWidth();
	float getHeight();

	void setWidthAndHeight(float width, float height);
	HRESULT createDirect3DContext(HWND wndHandle);
	void initializeCamAndParticles(float width, float height, float nearPlane, float farPlane);
	void reInitializeCam(float width, float height, float nearPlane, float farPlane);
	void reportLiveObjects();

	void release();
};

