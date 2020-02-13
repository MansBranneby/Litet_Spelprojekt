#include "ShadowMapping.h"

ShadowMapping::ShadowMapping()
{
	m_shadowVS = VertexShader(L"VertexShaderShadowMapping.hlsl");
	m_shadowCam = new Camera(DX::getInstance()->getWidth(), DX::getInstance()->getHeight(), 10.0f, 250.0f, XMVectorSet(100.0f, 80.0f, -50.0f, 0.0f));

	D3D11_TEXTURE2D_DESC descTex;
	ZeroMemory(&descTex, sizeof(descTex));
	descTex.Width = (UINT)DX::getInstance()->getWidth();
	descTex.Height = (UINT)DX::getInstance()->getHeight();
	descTex.MipLevels = descTex.ArraySize = 1;
	descTex.Format = DXGI_FORMAT_R32_TYPELESS;
	descTex.SampleDesc.Count = 1;
	descTex.SampleDesc.Quality = 0;
	descTex.Usage = D3D11_USAGE_DEFAULT;
	descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descTex.CPUAccessFlags = 0;
	descTex.MiscFlags = 0;

	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&descTex, NULL, &m_shadowTex);
	if (FAILED(hr))
		MessageBox(NULL, L"shadowTex in shadowmapping", L"Error", MB_OK | MB_ICONERROR);

	if (m_shadowTex != 0)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = DX::getInstance()->getDevice()->CreateDepthStencilView(m_shadowTex, &descDSV, &m_shadowDSV);
		if (FAILED(hr))
			MessageBox(NULL, L"shadowDSV in shadowmapping", L"Error", MB_OK | MB_ICONERROR);

		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = descTex.MipLevels;
		descSRV.Texture2D.MostDetailedMip = 0;

		hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_shadowTex, &descSRV, &m_shadowSRV);
		if (FAILED(hr))
			MessageBox(NULL, L"shadowSRV in shadowmapping", L"Error", MB_OK | MB_ICONERROR);
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = DX::getInstance()->getDevice()->CreateSamplerState(&sampDesc, &m_shadowSampler);
	if (FAILED(hr))
		MessageBox(NULL, L"sampler in shadowmapping", L"Error", MB_OK | MB_ICONERROR);
}

ShadowMapping::~ShadowMapping()
{
	m_shadowVS.release();
	delete m_shadowCam;

	m_shadowTex->Release();
	m_shadowSampler->Release();
	m_shadowDSV->Release();
	m_shadowSRV->Release();
}

VertexShader ShadowMapping::getVertexShader()
{
	return m_shadowVS;
}

Camera* ShadowMapping::getCamera()
{
	return m_shadowCam;
}

ID3D11SamplerState* ShadowMapping::getSamplerState()
{
	return m_shadowSampler;
}

ID3D11DepthStencilView* ShadowMapping::getDepthStencilView()
{
	return m_shadowDSV;
}

ID3D11ShaderResourceView* ShadowMapping::getShaderResourceView()
{
	return m_shadowSRV;
}
