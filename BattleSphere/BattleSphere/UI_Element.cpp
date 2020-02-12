#include "UI_Element.h"

void UI_Element::initializeResources(std::wstring fileName)
{
	// Vertex list //
	m_vertexList[0] =
	{
		-1.0f, 1.0f, 0.0f,	//v0 pos	L T
		0.0f, 0.0f,			//v0 tex
		0.0f, 0.0f, 0.0f,
	};
	m_vertexList[1] =
	{
		1.0, -1.0f, 0.0f,	//v1 pos	R B
		1.0f, 1.0f,			//v1 tex
		0.0f, 0.0f, 0.0f,
	};
	m_vertexList[2] =
	{
		-1.0f, -1.0f, 0.0f, //v2 pos	L B
		0.0f, 1.0f,			//v2 tex
		0.0f, 0.0f, 0.0f,
	};
	m_vertexList[3] =
	{
		-1.0f, 1.0f, 0.0f,	//v3 pos	L T
		0.0f, 0.0f,			//v3 tex
		0.0f, 0.0f, 0.0f,
	};
	m_vertexList[4] =
	{
		1.0f, 1.0f, 0.0f,	//v4 pos	R T
		1.0f, 0.0f,			//v4 tex
		0.0f, 0.0f, 0.0f,
	};
	m_vertexList[5] =
	{
		1.0f, -1.0f, 0.0f,	//v5 pos	R B
		1.0f, 1.0f,			//v5 tex
		0.0f, 0.0f, 0.0f
	};

	// Vertexbuffer //
	m_constantBuffer = new ConstantBuffer(m_vertexList, sizeof(vertex) * 6);
	
	std::wstring wName(fileName);
	const wchar_t* wcName = wName.c_str();
	HRESULT hr = CreateWICTextureFromFile(DX::getInstance()->getDevice(), wcName, NULL, &m_elementSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"Error in m_elementSRV", L"Error", MB_OK | MB_ICONERROR);
}

UI_Element::UI_Element(std::wstring fileName, float posX, float posY, float sizeX, float sizeY)
{
	m_posX = posX;
	m_posY = posY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	initializeResources(fileName);
}

UI_Element::~UI_Element()
{
	delete m_constantBuffer;
	m_elementSRV->Release();
	m_texture->Release();
}

void UI_Element::updateConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(*m_constantBuffer->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_vertexList, sizeof(vertex) * 6);
	DX::getInstance()->getDeviceContext()->Unmap(*m_constantBuffer->getConstantBuffer(), 0);
}

void UI_Element::draw()
{
	UINT32 vertexSize = sizeof(vertex);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, m_constantBuffer->getConstantBuffer(), &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->Draw(6, 0);
}
