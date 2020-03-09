#include "DX.h"
#include "Particles.h"


DX* DX::m_instance = nullptr;

DX::DX()
{
	m_debug = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_swapChain = nullptr;
	m_pDSStateEnabled = nullptr;
	m_pDSStateDisabled = nullptr;
	m_camera = nullptr;
	m_particles = nullptr;
}

void DX::createStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create depth stencil state
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	HRESULT hr = DX::getInstance()->getDevice()->CreateDepthStencilState(&dsDesc, &m_pDSStateDisabled);
	if (FAILED(hr))
		MessageBox(NULL, L"pDSState", L"Error", MB_OK | MB_ICONERROR);

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;


	hr = DX::getInstance()->getDevice()->CreateDepthStencilState(&dsDesc, &m_pDSStateEnabled);
	if (FAILED(hr))
		MessageBox(NULL, L"pDSState", L"Error", MB_OK | MB_ICONERROR);
}

DX* DX::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new DX();
	}

	return m_instance;
}

Camera* DX::getCam()
{
	return m_camera;
}

Particles* DX::getParticles()
{
	return m_particles;
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

ID3D11DepthStencilState* DX::getDSSEnabled()
{
	return m_pDSStateEnabled;
}

ID3D11DepthStencilState* DX::getDSSDisabled()
{
	return m_pDSStateDisabled;
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
	scd.Windowed = true;                                    // windowed/full-screen mode

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
	hr = m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));
	createStencilStates();
	return hr;
}

void DX::initializeCamAndParticles(float width, float height, float nearPlane, float farPlane)
{
	m_camera = new Camera();
	m_camera->initialize(width, height, nearPlane, farPlane);

	// Initialize particles
	m_particles = new Particles;
}


void DX::reportLiveObjects()
{
	m_debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
}



void DX::release()
{
	m_swapChain->SetFullscreenState(false, NULL);
	if (m_camera) delete m_camera;
	if (m_particles) delete m_particles;
	getDeviceContext()->Release();
	getSwapChain()->Release();
	m_pDSStateDisabled->Release();
	m_pDSStateEnabled->Release();
	if(m_debug)	m_debug->Release();
	getDevice()->Release();
}
