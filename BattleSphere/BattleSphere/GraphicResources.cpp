#include "GraphicResources.h"

HWND GraphicResources::initializeResources(HINSTANCE hInstance)
{
	HWND wndHandle = initWindow(hInstance);
	DX::getInstance()->createDirect3DContext(wndHandle);
	float width = DX::getInstance()->getWidth();
	float height = DX::getInstance()->getHeight();
	float nearPlane = 0.1f;
	float farPlane = 500.0f;
	DX::getInstance()->initializeCam(width, height, nearPlane, farPlane);

	createDepthStencil();
	createBackBuffer();
	createBlendState();
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

	// TODO: Wutface?
	int dab = GetClientRect(handle, &rc);

	return handle;
}

// TODO: Not sure if it's suppose to be here
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam); // Tells the compiler this function exists
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//// check if IMGUI can handle the message (when we click INSIDE ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

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
	descDepth.Width = (UINT)m_viewPort.Width;//(UINT)DX::getInstance()->getWidth();
	descDepth.Height = (UINT)m_viewPort.Height;//(UINT)DX::getInstance()->getHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS; // D24_UNORM_S8_UINT; // D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, L"pDepthStencil", L"Error", MB_OK | MB_ICONERROR);


	// Bind depth stencil state

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	if(pDepthStencil != nullptr)
		hr = DX::getInstance()->getDevice()->CreateDepthStencilView(pDepthStencil, &descDSV, &m_depthDSV);
	if (FAILED(hr))
		MessageBox(NULL, L"_depthStencilView", L"Error", MB_OK | MB_ICONERROR);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	
	srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	
	// Create the depth stencil view
	if (pDepthStencil != nullptr)
		hr = DX::getInstance()->getDevice()->CreateShaderResourceView(pDepthStencil, &srvDesc, &m_depthSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"_depthStencilView", L"Error", MB_OK | MB_ICONERROR);


	
	pDepthStencil->Release();

}

void GraphicResources::createBackBuffer()
{
	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	DX::getInstance()->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	if(pBackBuffer != nullptr)
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
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DX::getInstance()->getDevice()->CreateSamplerState(&sampDesc, &m_samplerState);
	if (FAILED(hr))
		MessageBox(NULL, L"m_samplerState", L"Error", MB_OK | MB_ICONERROR);
}

void GraphicResources::createBlendState()
{
	D3D11_BLEND_DESC blendStateDescription = {};
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	DX::getInstance()->getDevice()->CreateBlendState(&blendStateDescription, &m_blendState);
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
	m_depthDSV = nullptr;
	m_backbufferRTV = nullptr;
	m_samplerState = nullptr;
	m_depthSRV = nullptr;
}

GraphicResources::~GraphicResources()
{
	if (m_rasterizerState)
		m_rasterizerState->Release();
	if (m_blendState)
		m_blendState->Release();
	if (m_depthDSV)
		m_depthDSV->Release();
	if (m_backbufferRTV)
		m_backbufferRTV->Release();
	if (m_samplerState)
		m_samplerState->Release();
	if (m_depthSRV)
		m_depthSRV->Release();
	
}

void GraphicResources::bindDepthStencilState()
{
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(1, 1, &m_depthSRV);
}

ID3D11RasterizerState* GraphicResources::getRasterizerState() const
{
	return m_rasterizerState;
}

ID3D11DepthStencilView* GraphicResources::getDepthStencilView() const
{
	return m_depthDSV;
}

ID3D11RenderTargetView** GraphicResources::getBackBuffer()
{
	return &m_backbufferRTV;
}

ID3D11SamplerState** GraphicResources::getSamplerState()
{
	return &m_samplerState;
}

ID3D11BlendState* GraphicResources::getBlendState() const
{
	return m_blendState;
}

void GraphicResources::setViewPortDim(UINT width, UINT height)
{
	m_viewPort.Width = (float)width;
	m_viewPort.Height = (float)height;
	DX::getInstance()->getDeviceContext()->RSSetViewports(1, &m_viewPort);

}
