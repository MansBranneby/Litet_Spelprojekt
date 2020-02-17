#include "DX.h"

DX* DX::m_instance = nullptr;

DX::DX()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_swapChain = nullptr;
}

DX* DX::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new DX();
	}


	return m_instance;
}

ID3D11Device* DX::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* DX::getDeviceContext() 
{
	return m_deviceContext;
}

IDXGISwapChain* DX::getSwapChain() 
{
	return m_swapChain;
}

float DX::getWidth()
{
	return m_width;
}

float DX::getHeight()
{
	return m_height;
}

HRESULT DX::createDirect3DContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Height = (UINT)m_height;
	scd.BufferDesc.Width = (UINT)m_width;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };

	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&scd,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext);


	return hr;
}

void DX::release()
{
	getDevice()->Release();
	getDeviceContext()->Release();
	getSwapChain()->Release();
}
