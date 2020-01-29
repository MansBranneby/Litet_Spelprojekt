#include "GraphicResources.h"

HWND GraphicResources::initializeResources(HINSTANCE hInstance)
{
	HWND wndHandle = initWindow(hInstance);
	DX::getInstance()->createDirect3DContext(wndHandle);

	createDepthStencil();
	createBackBuffer();
	setRasterizerState();
	setSamplerState();
	setViewPort();

	return wndHandle;
}

HWND GraphicResources::initWindow(HINSTANCE hInstance)
{
	// TODO Not sure if it's suppose to be here
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BattleSphere";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)DX::getInstance()->getWidth() , (int)DX::getInstance()->getHeight() };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND handle = CreateWindow(
		L"BattleSphere",
		L"Battle Sphere",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// TODO Wutface?
	int dab = GetClientRect(handle, &rc);

	return handle;
}

// TODO Not sure if it's suppose to be here
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//// check if IMGUI can handle the message (when we click INSIDE ImGui
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	//	return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void GraphicResources::createDepthStencil()
{
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_viewPort.Width;//(UINT)DX::getInstance()->getWidth();
	descDepth.Height = m_viewPort.Height;//(UINT)DX::getInstance()->getHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, L"pDepthStencil", L"Error", MB_OK | MB_ICONERROR);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	hr = DX::getInstance()->getDevice()->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr))
		MessageBox(NULL, L"pDSState", L"Error", MB_OK | MB_ICONERROR);

	// Bind depth stencil state
	DX::getInstance()->getDeviceContext()->OMSetDepthStencilState(pDSState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	if(pDepthStencil != nullptr)
		hr = DX::getInstance()->getDevice()->CreateDepthStencilView(pDepthStencil, &descDSV, &m_depthStencilView);
	if (FAILED(hr))
		MessageBox(NULL, L"_depthStencilView", L"Error", MB_OK | MB_ICONERROR);

	if(pDepthStencil != nullptr)
		pDepthStencil->Release();
	if(pDSState != nullptr)
		pDSState->Release();
}

void GraphicResources::createBackBuffer()
{
	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	DX::getInstance()->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	DX::getInstance()->getDevice()->CreateRenderTargetView(pBackBuffer, NULL, &m_backbufferRTV);
	pBackBuffer->Release();
}

void GraphicResources::setViewPort()
{
	DX::getInstance()->getDeviceContext()->RSSetViewports(1, &m_viewPort);
}

void GraphicResources::setRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	HRESULT hr = DX::getInstance()->getDevice()->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(hr))
		MessageBox(NULL, L"_rasterizerState", L"Error", MB_OK | MB_ICONERROR);
}

void GraphicResources::setSamplerState()
{
	HRESULT hr = E_FAIL;

	// SAMPLERS //
	ID3D11SamplerState* pointClamp;
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DX::getInstance()->getDevice()->CreateSamplerState(&sampDesc, &pointClamp);
	if (FAILED(hr))
		MessageBox(NULL, L"_samplerState", L"Error", MB_OK | MB_ICONERROR);


	ID3D11SamplerState* linearWrap;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DX::getInstance()->getDevice()->CreateSamplerState(&sampDesc, &linearWrap);
	if (FAILED(hr))
		MessageBox(NULL, L"_samplerState", L"Error", MB_OK | MB_ICONERROR);

	// Set samplers
	DX::getInstance()->getDeviceContext()->VSSetSamplers(0, 1, &pointClamp);
	DX::getInstance()->getDeviceContext()->DSSetSamplers(0, 1, &pointClamp);
	DX::getInstance()->getDeviceContext()->PSSetSamplers(0, 1, &linearWrap);
	DX::getInstance()->getDeviceContext()->PSSetSamplers(1, 1, &pointClamp);
	// release pointers to sampler states
	pointClamp->Release();
	linearWrap->Release();
}

GraphicResources::GraphicResources()
{
	m_viewPort.Width = DX::getInstance()->getWidth();
	m_viewPort.Height = DX::getInstance()->getHeight();
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;

	m_rasterizerState = nullptr;
	m_depthStencilView = nullptr;
	m_backbufferRTV = nullptr;
	m_samplerState = nullptr;
}

GraphicResources::~GraphicResources()
{
	if (m_rasterizerState)
		m_rasterizerState->Release();
	if (m_depthStencilView)
		m_depthStencilView->Release();
	if (m_backbufferRTV)
		m_backbufferRTV->Release();
	if (m_samplerState)
		m_samplerState->Release();
}

ID3D11RasterizerState* GraphicResources::getRasterizerState() const
{
	return m_rasterizerState;
}

ID3D11DepthStencilView* GraphicResources::getDepthStencilView() const
{
	return m_depthStencilView;
}

ID3D11RenderTargetView** GraphicResources::getBackBuffer()
{
	return &m_backbufferRTV;
}

ID3D11SamplerState** GraphicResources::getSamplerState()
{
	return &m_samplerState;
}