#include "GeometryShader.h"

// Help function to create VertexBuffer
HRESULT GeometryShader::createGeometryShader(LPCWSTR fileName, ID3DBlob** pGS, ID3DBlob** errorBlob)
{
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "GS_main", "gs_4_0", D3DCOMPILE_DEBUG, 0, pGS, errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)(*errorBlob)->GetBufferPointer());
			(*errorBlob)->Release();
		}
		if (pGS)
			(*pGS)->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createGeometryShader in GeometryShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	DX::getInstance()->getDevice()->CreateGeometryShader((*pGS)->GetBufferPointer(), (*pGS)->GetBufferSize(), nullptr, &m_geometryShader);
	return result;
}

HRESULT GeometryShader::createSOGeometryShader(LPCWSTR fileName, ID3DBlob** pGS, ID3DBlob** errorBlob)
{
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "GS_main", "gs_4_0", D3DCOMPILE_DEBUG, 0, pGS, errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)(*errorBlob)->GetBufferPointer());
			(*errorBlob)->Release();
		}
		if (pGS)
			(*pGS)->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createGeometryShader in GeometryShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	D3D11_SO_DECLARATION_ENTRY pDecl[] =
	{
		// Stream, semantic name, semantic index, start component, component count, output slot
		{ 0, "VERTEX_ID", 0, 0, 1, 0 }
	};

	HRESULT hr = DX::getInstance()->getDevice()->CreateGeometryShaderWithStreamOutput(
		(*pGS)->GetBufferPointer(),
		(*pGS)->GetBufferSize(),
		pDecl,
		1, 
		NULL, 
		0, 
		D3D11_SO_NO_RASTERIZED_STREAM,
		NULL, 
		&m_geometryShader);
	return result;
}

GeometryShader::GeometryShader()
{
	m_geometryShader = nullptr;
}

// Constructor
GeometryShader::GeometryShader(LPCWSTR fileName, int type)
{
	ID3DBlob* pGS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	switch (type)
	{
	case 1:
		createSOGeometryShader(fileName, &pGS, &errorBlob);
		break;

	default:
	createGeometryShader(fileName, &pGS, &errorBlob);
		break;
	}

	// Release "refence" to pGS and errorBlob interface object
	if (pGS) pGS->Release();
	if (errorBlob) errorBlob->Release();
}

ID3D11GeometryShader& GeometryShader::getGeometryShader() const
{
	return *m_geometryShader;
}

void GeometryShader::release()
{
	if (m_geometryShader) m_geometryShader->Release();
}
