#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "DX.h"
#include "ComputeShader.h"

using namespace DirectX;

class Bloom
{
private:
	//ID3D11Texture2D* m_materialTex = nullptr;
	//ID3D11Texture2D* m_horizontalTex = nullptr;
	ID3D11Texture2D* m_sceneTex = nullptr;
	ID3D11Texture2D* m_bloomTex = nullptr;
	ID3D11Texture2D* m_horizontalTex = nullptr;

	ID3D11RenderTargetView* m_bloomRTV[2] = {};

	ID3D11ShaderResourceView* m_materialSRV = nullptr;
	ID3D11ShaderResourceView* m_horizontalSRV = nullptr;
	ID3D11ShaderResourceView* m_bloomSRV[2] = {};

	ID3D11UnorderedAccessView* m_horizontalUAV = nullptr;
	ID3D11UnorderedAccessView* m_bloomUAV = nullptr;

	ComputeShader m_horizontalCS;
	ComputeShader m_verticalCS;

	void createResources();
	void createShaders();

public:
	Bloom();
	~Bloom();

	void setRenderTarget();
	void setShaderResource();
	void clearRenderTarget();
	void run();
};

