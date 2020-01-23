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

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;

public:
	static DX* getInstance();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	IDXGISwapChain* getSwapChain();
	HRESULT createDirect3DContext(HWND wndHandle);
};

