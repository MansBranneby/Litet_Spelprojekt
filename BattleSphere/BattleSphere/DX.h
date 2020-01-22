#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
using namespace DirectX;

class DX 
{
private:
	static ID3D11Device* m_device;
	static ID3D11DeviceContext* m_deviceContext;
	static IDXGISwapChain* m_swapChain;

	DX();
public:
	static ID3D11Device* getDevice();
	static ID3D11DeviceContext* getDeviceContext();
	static IDXGISwapChain* getSwapChain();
	static HRESULT createDirect3DContext(HWND wndHandle);
};

