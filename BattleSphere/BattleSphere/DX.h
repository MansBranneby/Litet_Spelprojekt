#pragma once
#include <d3d11.h>
//#include <d3dcompiler.h>
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")

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
	float m_width;
	float m_height;

public:
	static DX* getInstance();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	IDXGISwapChain* getSwapChain();

	float getWidth();
	float getHeight();

	HRESULT createDirect3DContext(HWND wndHandle);

	void release();
};

