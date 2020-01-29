#include "Bloom.h"

void Bloom::createResources()
{
	//// TEXTURES ////

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = DX::getInstance()->getWidth();
	texDesc.Height = DX::getInstance()->getHeight();
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;

	// Material
	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDesc, NULL, &m_materialTex);
	if (FAILED(hr))
		MessageBox(NULL, L"materialTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Horizontal
	hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDesc, NULL, &m_horizontalTex);
	if (FAILED(hr))
		MessageBox(NULL, L"horizontalTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Bloom
	hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDesc, NULL, &m_bloomTex);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	//// RENDER TARGETS ////

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));
	RTVDesc.Format = texDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	// Material
	hr = DX::getInstance()->getDevice()->CreateRenderTargetView(m_materialTex, &RTVDesc, &m_materialRTV);
	if (FAILED(hr))
		MessageBox(NULL, L"materialRTV in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Horizontal
	hr = DX::getInstance()->getDevice()->CreateRenderTargetView(m_horizontalTex, &RTVDesc, &m_horizontalRTV);
	if (FAILED(hr))
		MessageBox(NULL, L"horizontalRTV in Bloom", L"Error", MB_OK | MB_ICONERROR);
	// Bloom
	hr = DX::getInstance()->getDevice()->CreateRenderTargetView(m_bloomTex, &RTVDesc, &m_bloomRTV);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomRTV in Bloom", L"Error", MB_OK | MB_ICONERROR);

	//// SHADER RESOURCES ////

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = texDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// Material
	hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_materialTex, &SRVDesc, &m_materialSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"materialSRV in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Horizontal
	hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_horizontalTex, &SRVDesc, &m_horizontalSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"horizontalSRV in Bloom", L"Error", MB_OK | MB_ICONERROR);
	// Bloom
	hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_bloomTex, &SRVDesc, &m_bloomSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomSRV in Bloom", L"Error", MB_OK | MB_ICONERROR);
}

Bloom::Bloom()
{
	createResources();
}

Bloom::~Bloom()
{
}

ID3D11RenderTargetView* Bloom::getMaterialRTV()
{
	return m_materialRTV;
}

ID3D11ShaderResourceView* Bloom::getBloomSRV()
{
	return m_bloomSRV;
}
