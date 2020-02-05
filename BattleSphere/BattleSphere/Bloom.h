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
	ID3D11Texture2D* m_sceneTex = nullptr;
	ID3D11Texture2D* m_bloomFullResTex = nullptr;
	ID3D11Texture2D* m_bloomTex = nullptr;
	ID3D11Texture2D* m_gaussTex = nullptr;

	ID3D11RenderTargetView* m_bloomLowResRTV = nullptr;			// Bloom materials in low resolution
	ID3D11RenderTargetView* m_bloomRTV[2] = {};					// Scene and bloom materials in full resolution

	ID3D11ShaderResourceView* m_bloomFullResSRV = nullptr;		// Bloom materials in full resolution
	ID3D11ShaderResourceView* m_gaussSRV = nullptr;				// SRV and UAV in middle
	ID3D11ShaderResourceView* m_bloomSRV[2] = {};				// Scene and bloom in low 

	ID3D11UnorderedAccessView* m_gaussUAV = nullptr;			// SRV and UAV in middle
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

	ID3D11RenderTargetView** getRenderTargets();

	void run();
};

