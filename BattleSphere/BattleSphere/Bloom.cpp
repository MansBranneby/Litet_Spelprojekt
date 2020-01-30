#include "Bloom.h"

void Bloom::createResources()
{
	//// TEXTURES ////

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = (UINT)DX::getInstance()->getWidth();
	texDesc.Height = (UINT)DX::getInstance()->getHeight();
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
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


	if (m_materialTex != 0 && m_horizontalTex != 0 && m_bloomTex != 0)
	{
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

		//// UNORDERED ACCESS VIEWS ////

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
		ZeroMemory(&UAVdesc, sizeof(UAVdesc));
		UAVdesc.Format = texDesc.Format;
		UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		UAVdesc.Texture2D.MipSlice = 0;
		
		// Horizontal
		hr = DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_horizontalTex, &UAVdesc, &m_horizontalUAV);
		if (FAILED(hr))
			MessageBox(NULL, L"horizontalUAV in Bloom", L"Error", MB_OK | MB_ICONERROR);
	}

}

void Bloom::createShaders()
{
	m_horizontalCS = ComputeShader(L"ComputeShaderHGaussian.hlsl");
	m_verticalCS = ComputeShader(L"ComputeShaderVGaussian.hlsl");
}

Bloom::Bloom()
{
	createResources();
	createShaders();
}

Bloom::~Bloom()
{
	m_materialTex->Release();
	m_horizontalTex->Release();
	m_bloomTex->Release();

	m_materialRTV->Release();
	m_horizontalRTV->Release();
	m_bloomRTV->Release();

	m_materialSRV->Release();
	m_horizontalSRV->Release();
	m_bloomSRV->Release();

	m_horizontalUAV->Release();

	m_horizontalCS.release();
	m_verticalCS.release();
}

ID3D11RenderTargetView* Bloom::getMaterialRTV()
{
	return m_materialRTV;
}

ID3D11ShaderResourceView* Bloom::getBloomSRV()
{
	return m_bloomSRV;
}

void Bloom::setRenderTarget()
{
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(1, &m_materialRTV, NULL);
}

void Bloom::setShaderResource()
{
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &m_horizontalSRV);
}

void Bloom::run()
{
	ID3D11RenderTargetView* nullRTV = { NULL };
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(0, &nullRTV, NULL);

	DX::getInstance()->getDeviceContext()->CSSetShader(&m_horizontalCS.getComputeShader(), nullptr, 0);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_materialSRV);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_horizontalUAV, 0);
	DX::getInstance()->getDeviceContext()->Dispatch(2, (UINT)DX::getInstance()->getHeight() / 2, 1);

	ID3D11ShaderResourceView* nullSRV = { NULL };
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &nullSRV);
}
