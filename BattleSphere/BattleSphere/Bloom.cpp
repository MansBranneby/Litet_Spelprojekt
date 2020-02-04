#include "Bloom.h"

void Bloom::createResources()
{
	//// TEXTURES ////

	D3D11_TEXTURE2D_DESC texDescRS;
	ZeroMemory(&texDescRS, sizeof(texDescRS));
	texDescRS.Width = (UINT)DX::getInstance()->getWidth();
	texDescRS.Height = (UINT)DX::getInstance()->getHeight();
	texDescRS.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescRS.MipLevels = 1;
	texDescRS.ArraySize = 1;
	texDescRS.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescRS.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDescRS.MiscFlags = 0;
	texDescRS.SampleDesc.Count = 1;

	// TODO: Ta bort alpha value?

	D3D11_TEXTURE2D_DESC texDescRSU;
	ZeroMemory(&texDescRSU, sizeof(texDescRSU));
	texDescRSU.Width = (UINT)DX::getInstance()->getWidth();
	texDescRSU.Height = (UINT)DX::getInstance()->getHeight();
	texDescRSU.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescRSU.MipLevels = 1;
	texDescRSU.ArraySize = 1;
	texDescRSU.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDescRSU.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDescRSU.MiscFlags = 0;
	texDescRSU.SampleDesc.Count = 1;

	D3D11_TEXTURE2D_DESC texDescGauss;
	ZeroMemory(&texDescGauss, sizeof(texDescGauss));
	texDescGauss.Width = (UINT)DX::getInstance()->getWidth();
	texDescGauss.Height = (UINT)DX::getInstance()->getHeight();
	texDescGauss.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescGauss.MipLevels = 1;
	texDescGauss.ArraySize = 1;
	texDescGauss.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDescGauss.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	texDescGauss.MiscFlags = 0;
	texDescGauss.SampleDesc.Count = 1;

	//// Material
	//HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDesc, NULL, &m_materialTex);
	//if (FAILED(hr))
	//	MessageBox(NULL, L"materialTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Scene
	HRESULT hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDescRS, NULL, &m_sceneTex);
	if (FAILED(hr))
		MessageBox(NULL, L"sceneTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Bloom
	hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDescRSU, NULL, &m_bloomTex);
	if (FAILED(hr))
		MessageBox(NULL, L"bloomTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	// Gauss
	hr = DX::getInstance()->getDevice()->CreateTexture2D(&texDescGauss, NULL, &m_gaussTex);
	if (FAILED(hr))
		MessageBox(NULL, L"gaussTex in Bloom", L"Error", MB_OK | MB_ICONERROR);

	if (m_sceneTex != 0 && m_bloomTex != 0 && m_gaussTex != 0)
	{
		//// RENDER TARGETS ////

		D3D11_RENDER_TARGET_VIEW_DESC RTVDescRS;
		ZeroMemory(&RTVDescRS, sizeof(RTVDescRS));
		RTVDescRS.Format = texDescRS.Format;
		RTVDescRS.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDescRS.Texture2D.MipSlice = 0;

		D3D11_RENDER_TARGET_VIEW_DESC RTVDescRSU;
		ZeroMemory(&RTVDescRSU, sizeof(RTVDescRSU));
		RTVDescRSU.Format = texDescRSU.Format;
		RTVDescRSU.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDescRSU.Texture2D.MipSlice = 0;

		// 0: Scene, 1: Emissive materials
		hr = DX::getInstance()->getDevice()->CreateRenderTargetView(m_sceneTex, &RTVDescRS, &m_bloomRTV[0]);
		if (FAILED(hr))
			MessageBox(NULL, L"bloomRTV[0] in Bloom", L"Error", MB_OK | MB_ICONERROR);

		hr = DX::getInstance()->getDevice()->CreateRenderTargetView(m_bloomTex, &RTVDescRSU, &m_bloomRTV[1]);
		if (FAILED(hr))
			MessageBox(NULL, L"bloomRTV[1] in Bloom", L"Error", MB_OK | MB_ICONERROR);

		//// SHADER RESOURCES ////

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescRS;
		ZeroMemory(&SRVDescRS, sizeof(SRVDescRS));
		SRVDescRS.Format = texDescRS.Format;
		SRVDescRS.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDescRS.Texture2D.MostDetailedMip = 0;
		SRVDescRS.Texture2D.MipLevels = texDescRS.MipLevels;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescRSU;
		ZeroMemory(&SRVDescRSU, sizeof(SRVDescRSU));
		SRVDescRSU.Format = texDescRSU.Format;
		SRVDescRSU.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDescRSU.Texture2D.MostDetailedMip = 0;
		SRVDescRSU.Texture2D.MipLevels = texDescRSU.MipLevels;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescGauss;
		ZeroMemory(&SRVDescGauss, sizeof(SRVDescGauss));
		SRVDescGauss.Format = texDescGauss.Format;
		SRVDescGauss.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDescGauss.Texture2D.MostDetailedMip = 0;
		SRVDescGauss.Texture2D.MipLevels = texDescGauss.MipLevels;

		//// Material
		//hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_bloomTex, &SRVDesc, &m_materialSRV);
		//if (FAILED(hr))
		//	MessageBox(NULL, L"materialSRV in Bloom", L"Error", MB_OK | MB_ICONERROR);

		// Gauss
		hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_gaussTex, &SRVDescGauss, &m_gaussSRV);
		if (FAILED(hr))
			MessageBox(NULL, L"gaussSRV in Bloom", L"Error", MB_OK | MB_ICONERROR);
		
		// Bloom - 0: Scene, 1: Emissive materials
		hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_sceneTex, &SRVDescRS, &m_bloomSRV[0]);
		if (FAILED(hr))
			MessageBox(NULL, L"bloomSRV[0] in Bloom", L"Error", MB_OK | MB_ICONERROR);
		
		hr = DX::getInstance()->getDevice()->CreateShaderResourceView(m_bloomTex, &SRVDescRSU, &m_bloomSRV[1]);
		if (FAILED(hr))
			MessageBox(NULL, L"bloomSRV[1] in Bloom", L"Error", MB_OK | MB_ICONERROR);

		//// UNORDERED ACCESS VIEWS ////

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
		ZeroMemory(&UAVdesc, sizeof(UAVdesc));
		UAVdesc.Format = texDescRSU.Format;
		UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		UAVdesc.Texture2D.MipSlice = 0;

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdescGauss;
		ZeroMemory(&UAVdescGauss, sizeof(UAVdescGauss));
		UAVdescGauss.Format = texDescGauss.Format;
		UAVdescGauss.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		UAVdescGauss.Texture2D.MipSlice = 0;
		
		// Gauss
		hr = DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_gaussTex, &UAVdescGauss, &m_gaussUAV);
		if (FAILED(hr))
			MessageBox(NULL, L"gaussUAV in Bloom", L"Error", MB_OK | MB_ICONERROR);

		// Bloom
		hr = DX::getInstance()->getDevice()->CreateUnorderedAccessView(m_bloomTex, &UAVdesc, &m_bloomUAV);
		if (FAILED(hr))
			MessageBox(NULL, L"bloomUAV in Bloom", L"Error", MB_OK | MB_ICONERROR);
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
	m_gaussTex->Release();
	m_sceneTex->Release();
	m_bloomTex->Release();

	m_bloomRTV[0]->Release();
	m_bloomRTV[1]->Release();

	//m_materialSRV->Release();
	m_gaussSRV->Release();
	m_bloomSRV[0]->Release();
	m_bloomSRV[1]->Release();

	m_gaussUAV->Release();
	m_bloomUAV->Release();

	m_horizontalCS.release();
	m_verticalCS.release();
}

void Bloom::setRenderTarget()
{
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(2, m_bloomRTV, NULL);
}

void Bloom::setShaderResource()
{
	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 2, m_bloomSRV);
}

void Bloom::clearRenderTarget()
{
	float clearColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	DX::getInstance()->getDeviceContext()->ClearRenderTargetView(m_bloomRTV[0], clearColour);
	DX::getInstance()->getDeviceContext()->ClearRenderTargetView(m_bloomRTV[1], clearColour);
}

ID3D11RenderTargetView** Bloom::getRenderTargets()
{
	return m_bloomRTV;
}

void Bloom::run()
{
	ID3D11RenderTargetView* nullRTV = { NULL };
	DX::getInstance()->getDeviceContext()->OMSetRenderTargets(0, &nullRTV, NULL);
	
	//// HORIZONTAL PASS ////
	DX::getInstance()->getDeviceContext()->CSSetShader(&m_horizontalCS.getComputeShader(), nullptr, 0);
	//DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_materialSRV);
	//DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_horizontalUAV, 0);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_bloomSRV[1]);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_gaussUAV, 0);
	DX::getInstance()->getDeviceContext()->Dispatch(60, (UINT)DX::getInstance()->getHeight(), 1); 

	ID3D11ShaderResourceView* nullSRV = { NULL };
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(1, 1, &nullSRV);

	ID3D11UnorderedAccessView* nullUAV = { NULL };
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, 0);

	//// VERTICAL PASS ////
	DX::getInstance()->getDeviceContext()->CSSetShader(&m_verticalCS.getComputeShader(), nullptr, 0);
	/*DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_horizontalSRV);*/
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &m_gaussSRV);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_bloomUAV, 0);
	DX::getInstance()->getDeviceContext()->Dispatch((UINT)DX::getInstance()->getWidth(), 36, 1);

	DX::getInstance()->getDeviceContext()->CSSetShaderResources(0, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->CSSetShaderResources(1, 1, &nullSRV);
	DX::getInstance()->getDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, 0);
}
