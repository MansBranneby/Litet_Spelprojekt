#include "ComputeShader.h"

HRESULT ComputeShader::createComputeShader(LPCWSTR fileName, ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "CS_main", "cs_5_0", D3DCOMPILE_DEBUG, 0, pVS, errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)(*errorBlob)->GetBufferPointer());
			(*errorBlob)->Release();
		}
		if (pVS)
			(*pVS)->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createComputeShader in ComputeShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	DX::getInstance()->getDevice()->CreateComputeShader((*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), nullptr, &m_computeShader);
	return result;
}

ComputeShader::ComputeShader()
{
	m_computeShader = nullptr;
}

ComputeShader::ComputeShader(LPCWSTR fileName)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	createComputeShader(fileName, &pVS, &errorBlob);

	// Release "refence" to pVS and errorBlob interface object
	if (pVS) pVS->Release();
	if (errorBlob) errorBlob->Release();
}

ID3D11ComputeShader& ComputeShader::getComputeShader() const
{
	return *m_computeShader;
}

void ComputeShader::release()
{
	if (m_computeShader) m_computeShader->Release();
}
