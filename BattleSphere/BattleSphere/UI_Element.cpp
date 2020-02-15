#include "UI_Element.h"

void UI_Element::initializeResources(std::wstring fileName)
{
	// Vertex list //
	float left, right, top, bottom, texU, texV;
	
	if (m_spriteSizeX != 0.0f || m_spriteSizeY != 0.0f)
	{
		left = m_posX - m_spriteSizeX / 2.0f;
		right = left + m_spriteSizeX;
		top = m_posY + m_spriteSizeY / 2.0f;
		bottom = top - m_spriteSizeY;

		texU = m_spriteSizeX / m_sizeX;
		texV = m_spriteSizeY / m_sizeY;
	}
	else
	{
		left = m_posX - m_sizeX / 2.0f;
		right = left + m_sizeX;
		top = m_posY + m_sizeY / 2.0f;
		bottom = top - m_sizeY;

		texU = 1.0f;
		texV = 1.0f;
	}

		m_vertexList[0] =
		{
			-1.0f, 1.0f, 0.0f,	//v0 pos	L T
			0.0f, 0.0f,			//v0 tex
			0.0f, 0.0f, 0.0f,
		};
		m_vertexList[1] =
		{
			1.0f, -1.0f, 0.0f,	//v1 pos	R B
			texU, texV,			//v1 tex
			0.0f, 0.0f, 0.0f,
		};
		m_vertexList[2] =
		{
			-1.0f, 1.0f, 0.0f,	//v2 pos	L B
			0.0f, texV,			//v2 tex
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
			texU, 0.0f,			//v4 tex
			0.0f, 0.0f, 0.0f,
		};
		m_vertexList[5] =
		{
			1.0f, -1.0f, 0.0f,	//v5 pos	R B
			texU, texV,			//v5 tex
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

UI_Element::UI_Element(std::wstring fileName, uiType type, float posX, float posY, float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames)
{
	m_type = type;

	m_posX = posX;
	m_posY = posY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_destinationX = m_posX;
	m_destinationY = m_posY;

	m_isReady = true;
	m_selectionTimer = 0.0f;

	m_spriteSizeX = spriteSizeX;
	m_spriteSizeY = spriteSizeY;

	initializeResources(fileName);

	switch (type)
	{
	case uiType::e_mainMenuSelection:
		m_animation = new UI_Animation(sizeX, sizeY, spriteSizeX, spriteSizeY, nrOfFrames, 1000.0f);
		break;
	case uiType::e_mainMenuBox:
		m_animation = new UI_Animation(sizeX, sizeY, spriteSizeX, spriteSizeY, nrOfFrames, 1000.0f);
		break;
	case uiType::e_static:
		break;
	default:
		break;
	}
}

UI_Element::~UI_Element()
{
	m_vertexBuffer->Release();
	m_elementSRV->Release();
	delete m_animation;
}

void UI_Element::updateElement(AnimationType animationType, float dt)
{
	switch (animationType)
	{
	case e_translate:
		if (m_destinationX != m_posX || m_destinationY != m_posY || !m_isReady) // Only update if element needs to move
		{

			m_isReady = m_animation->animateElement(animationType, m_vertexList, &m_posX, &m_posY, m_sizeX, m_sizeY, m_destinationX, m_destinationY, dt);

			updateVertexBuffer();
		}
		break;
	case e_sprite:
		m_animation->animateElement(animationType, m_vertexList, dt);
		updateVertexBuffer();
		break;
	default:
		break;
	}
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

float UI_Element::getPosX()
{
	return m_posX;
}

float UI_Element::getPosY()
{
	return m_posY;
}

void UI_Element::setDestinationX(float deltaX)
{
	m_destinationX += deltaX;
}

void UI_Element::setDestinationY(float deltaY)
{
	m_destinationY += deltaY;
}

void UI_Element::setReady(bool isReady)
{
	m_isReady = isReady;
}

bool UI_Element::isReady()
{
	//return m_posX == m_destinationX && m_posY == m_destinationY;
	return m_isReady;
}

void UI_Element::setSelectionTimer(float time)
{
	m_selectionTimer = time;
}
