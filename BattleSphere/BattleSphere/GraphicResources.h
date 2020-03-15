#pragma once
//#include "assimp/scene.h"
#include <d3d11.h>
#include "DX.h"
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")

class GraphicResources
{
private:
	D3D11_VIEWPORT m_viewPort;
	ID3D11RasterizerState* m_rasterizerState;

	// VIEWS //
	ID3D11DepthStencilView* m_depthDSV;
	ID3D11ShaderResourceView* m_depthSRV;
	ID3D11RenderTargetView* m_backbufferRTV;



	// SAMPLERS //
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11BlendState* m_blendState = nullptr;

	HWND initWindow(HINSTANCE hInstance);
	void createDepthStencil();
	void createBackBuffer();
	void setViewPort();
	void setRasterizerState();
	void setSamplerState();
	void createBlendState();
	void resizeBuffers();

public:

	// Member functions
	GraphicResources();
	~GraphicResources();
	void bindDepthStencilState();
	HWND initializeResources(HINSTANCE hInstance);
	void updateRenderTarget();
	ID3D11RasterizerState* getRasterizerState() const;
	ID3D11DepthStencilView* getDepthStencilView() const;
	ID3D11RenderTargetView** getBackBuffer();
	ID3D11SamplerState** getSamplerState();
	ID3D11BlendState* getBlendState() const;
	void setViewPortDim(UINT width, UINT height);
	void releaseDepthStencilViews();
};