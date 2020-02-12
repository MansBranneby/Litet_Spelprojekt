#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "DX.h"
#include "StructsAndEnums.h"

using namespace DirectX;


class ShadowMapping
{
private:
	ID3D11Texture2D* m_shadowTex = nullptr;

	ID3D11SamplerState* m_shadowSampler = nullptr;
	ID3D11DepthStencilView* m_shadowDSV = nullptr;
	ID3D11ShaderResourceView* m_shadowSRV = nullptr;

public:
	ShadowMapping();
	~ShadowMapping();

	ID3D11SamplerState* getSamplerState();
	ID3D11DepthStencilView* getDepthStencilView();
	ID3D11ShaderResourceView* getShaderResourceView();
};

