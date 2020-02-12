#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "DX.h"
#include "ComputeShader.h"
#include "StructsAndEnums.h"

using namespace DirectX;

class Bloom
{
private:
	bool m_fullResZero = false;
	ID3D11Texture2D* m_fullResTex[2] = { nullptr };
	ID3D11Texture2D* m_bloomTex = nullptr;
	ID3D11Texture2D* m_gaussTex = nullptr;

	ID3D11RenderTargetView* m_lowResRTV = nullptr;
	ID3D11RenderTargetView* m_fullResRTV[2] = { nullptr };

	ID3D11ShaderResourceView* m_gaussSRV= nullptr;				// SRV and UAV in middle
	ID3D11ShaderResourceView* m_bloomSRV[3] = { nullptr };				// Scene and bloom in low 

	ID3D11UnorderedAccessView* m_gaussUAV = nullptr ;			// SRV and UAV in middle
	ID3D11UnorderedAccessView* m_bloomUAV = nullptr;			// R,S,U

	ComputeShader m_horizontalCS;
	ComputeShader m_verticalCS;

	void createResources();
	void createShaders();

public:
	Bloom();
	~Bloom();

	void setRenderTarget(ID3D11DepthStencilView* depthStencil, renderPass pass);
	void setShaderResource(renderPass pass);
	void clearRenderTarget();
	void changeOrder();
	//ID3D11RenderTargetView** getRenderTargets();

	void run();
};

