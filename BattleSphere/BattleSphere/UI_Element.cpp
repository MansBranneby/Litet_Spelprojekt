#include "UI_Element.h"

void UI_Element::initializeResources(std::wstring fileName)
{
	// Vertex list //
	float left, right, top, bottom;
	
	left = m_posX - m_sizeX / 2.0f;
	right = left + m_sizeX;
	top = m_posY + m_sizeY / 2.0f;
	bottom = top - m_sizeY;

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

	m_vertexList[0].posX = left;
	m_vertexList[0].posY = top;

	m_vertexList[1].posX = right;
	m_vertexList[1].posY = bottom;

	m_vertexList[2].posX = left;
	m_vertexList[2].posY = bottom;

	m_vertexList[3].posX = left;
	m_vertexList[3].posY = top;

	m_vertexList[4].posX = right;
	m_vertexList[4].posY = top;

	m_vertexList[5].posX = right;
	m_vertexList[5].posY = bottom;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.ByteWidth = sizeof(vertex) * 6;
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = m_vertexList;
	DX::getInstance()->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	
	std::wstring wName(fileName);
	const wchar_t* wcName = wName.c_str();
	HRESULT hr = CoInitialize(NULL);
	hr = CreateWICTextureFromFile(DX::getInstance()->getDevice(), wcName, NULL, &m_elementSRV);
	if (FAILED(hr))
		MessageBox(NULL, L"Error in m_elementSRV", L"Error", MB_OK | MB_ICONERROR);
}

UI_Element::UI_Element(std::wstring fileName, float posX, float posY, float sizeX, float sizeY)
{
	m_posX = posX;
	m_posY = posY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_screenWidth = DX::getInstance()->getWidth();
	m_screenHeight = DX::getInstance()->getHeight();

	initializeResources(fileName);
}

UI_Element::~UI_Element()
{
	m_vertexBuffer->Release();
	m_elementSRV->Release();
	//m_texture->Release();
}

void UI_Element::updateElement(float posX, float posY)
{
	//if (posX != m_posX || posY != m_posY) // Has something happened?
	//{
	//	m_vertexList[0].
	//}
}

void UI_Element::updateVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedMemory;
	DX::getInstance()->getDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, m_vertexList, sizeof(vertex) * 6);
	DX::getInstance()->getDeviceContext()->Unmap(m_vertexBuffer, 0);
}

void UI_Element::draw()
{
	UINT32 vertexSize = sizeof(vertex);
	UINT32 offset = 0;

	DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &m_elementSRV);
	DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::getInstance()->getDeviceContext()->Draw(6, 0);
}
