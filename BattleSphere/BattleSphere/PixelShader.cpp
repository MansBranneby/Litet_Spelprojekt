#include "PixelShader.h"

HRESULT PixelShader::createPixelShader(LPCWSTR fileName)
{
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "PS_main", "ps_5_0", D3DCOMPILE_DEBUG, 0, &pPS, &errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createPixelShader in pixelShader.cpp has failed", MB_OK | MB_ICONERROR);

		
	}

	DX::getInstance()->getDevice()->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &m_pixelShader);

	if (pPS) pPS->Release();
	if (errorBlob) errorBlob->Release();

	return result;
}

PixelShader::PixelShader()
{
	m_pixelShader = nullptr;
}

PixelShader::PixelShader(LPCWSTR fileName)
{
	createPixelShader(fileName);
}

ID3D11PixelShader& PixelShader::getPixelShader() const
{
	return *m_pixelShader;
}