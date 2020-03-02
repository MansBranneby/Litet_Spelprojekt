#include "VertexShader.h"

// Help function to create VertexBuffer
HRESULT VertexShader::createVertexShader(LPCWSTR fileName, ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, pVS, errorBlob);

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
			MessageBox(NULL, L"Error", L"Error: createVertexShader in VertexShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	DX::getInstance()->getDevice()->CreateVertexShader((*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), nullptr, &m_vertexShader);
	return result;
}

void VertexShader::createInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORDS",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			20,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
	};

	DX::getInstance()->getDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), (*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), &m_vertexLayout);

	(*pVS)->Release();
}

void VertexShader::createBCInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	{
		"POSITION",		// "semantic" name in shader
		0,				// "semantic" index (not used)
		DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
		0,							 // input slot
		0,							 // offset of first element
		D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
		0							 // used for INSTANCING (ignore)
	},
	{
		"VERTEX_ID",
		0,				// same slot as previous (same vertexBuffer)
		DXGI_FORMAT_R32_UINT,
		0,
		12,							// offset of FIRST element (after POSITION)
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	}
	};

	DX::getInstance()->getDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), (*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), &m_vertexLayout);

	(*pVS)->Release();
}

void VertexShader::createPathInputLayout(ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
	{
		"POSITION",		// "semantic" name in shader
		0,				// "semantic" index (not used)
		DXGI_FORMAT_R32G32B32A32_FLOAT, // size of ONE element (3 floats)
		0,							 // input slot
		0,							 // offset of first element
		D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
		0							 // used for INSTANCING (ignore)
	}
	};

	DX::getInstance()->getDevice()->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), (*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), &m_vertexLayout);

	(*pVS)->Release();
}

VertexShader::VertexShader()
{
	m_vertexLayout = nullptr;
	m_vertexShader = nullptr;
}

// Constructor
VertexShader::VertexShader(LPCWSTR fileName, int type)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;
	createVertexShader(fileName, &pVS, &errorBlob);

	switch (type)
	{
	case 1:
		createBCInputLayout(&pVS, &errorBlob);
		break;
	case 2:
		createPathInputLayout(&pVS, &errorBlob);
		break;
	default:
		createInputLayout(&pVS, &errorBlob);
		break;
	}

	// Release "refence" to pVS and errorBlob interface object
	if (pVS) pVS->Release();
	if (errorBlob) errorBlob->Release();
}

ID3D11VertexShader& VertexShader::getVertexShader() const
{
	return *m_vertexShader;
}

ID3D11InputLayout& VertexShader::getvertexLayout() const
{
	return *m_vertexLayout;
}

void VertexShader::release()
{
	if (m_vertexLayout) m_vertexLayout->Release();
	if (m_vertexShader) m_vertexShader->Release();
}
