#include "ShadowMapping.h"

ShadowMapping::ShadowMapping()
{
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

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	HRESULT hr = DX::getInstance()->getDevice()->CreateDepthStencilView(m_shadowTex, &descDSV, &m_shadowDSV);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	descSRV.Format = DXGI_FORMAT_R32_FLOAT;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = descTex.MipLevels;
	descSRV.Texture2D.MostDetailedMip = 0;

	HRESULT hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_shadowTex, &descSRV, &m_shadowSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomTex in Bloom", L"Error", MB_OK | MB_ICONERROR);
}

ShadowMapping::~ShadowMapping()
{
	m_shadowTex->Release();

	m_shadowDSV->Release();
	m_shadowSRV->Release();
}

ID3D11DepthStencilView* ShadowMapping::getDepthStencilView()
{
	return m_shadowDSV;
}

ID3D11ShaderResourceView* ShadowMapping::getShaderResourceView()
{
	return m_shadowSRV;
}
