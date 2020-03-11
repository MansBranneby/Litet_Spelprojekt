#include "UI_Element.h"

void UI_Element::initializeResources(std::wstring fileName)
{
	// Vertex list //
	float left, right, top, bottom, texU, texV, alpha;

	(m_isDrawn ? alpha = 1.0f : alpha = 0.0f);
	
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
			alpha, 0.0f, 0.0f,
		};
		m_vertexList[1] =
		{
			1.0f, -1.0f, 0.0f,	//v1 pos	R B
			texU, texV,			//v1 tex
			alpha, 0.0f, 0.0f,
		};
		m_vertexList[2] =
		{
			-1.0f, 1.0f, 0.0f,	//v2 pos	L B
			0.0f, texV,			//v2 tex
			alpha, 0.0f, 0.0f,
		};
		m_vertexList[3] =
		{
			-1.0f, 1.0f, 0.0f,	//v3 pos	L T
			0.0f, 0.0f,			//v3 tex
			alpha, 0.0f, 0.0f,
		};
		m_vertexList[4] =
		{
			1.0f, 1.0f, 0.0f,	//v4 pos	R T
			texU, 0.0f,			//v4 tex
			alpha, 0.0f, 0.0f,
		};
		m_vertexList[5] =
		{
			1.0f, -1.0f, 0.0f,	//v5 pos	R B
			texU, texV,			//v5 tex
			alpha, 0.0f, 0.0f
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

UI_Element::UI_Element(std::wstring fileName, bool isDrawn, float posX, float posY, float sizeX, float sizeY, float spriteSizeX, float spriteSizeY, u_int nrOfFrames)
{
	//m_type = type;

	m_posX = posX;
	m_posY = posY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_destinationX = m_posX;
	m_destinationY = m_posY;

	m_alpha = isDrawn;
	m_isDrawn = isDrawn;
	m_isReady = true;
	m_selectionTimer = 0.0f;

	m_spriteSizeX = spriteSizeX;
	m_spriteSizeY = spriteSizeY;

	initializeResources(fileName);

	/*if(type != uiType::e_static)*/
	m_animation = new UI_Animation(sizeX, sizeY, spriteSizeX, spriteSizeY, nrOfFrames);

	/*switch (type)
	{
	case uiType::e_mainMenuSelection:
		m_animation = new UI_Animation(sizeX, sizeY, spriteSizeX, spriteSizeY, nrOfFrames);
		break;
	case uiType::e_mainMenuBox:
		m_animation = new UI_Animation(sizeX, sizeY, spriteSizeX, spriteSizeY, nrOfFrames);
		break;
	case uiType::e_static:
		break;
	default:
		break;
	}*/
}

UI_Element::~UI_Element()
{
	m_vertexBuffer->Release();
	m_elementSRV->Release();
	delete m_animation;
}

void UI_Element::updateElement(float dt)
{

	if (m_destinationX != m_posX || m_destinationY != m_posY || !m_isReady) // Translation
	{

		m_isReady = m_animation->translateElement(m_vertexList, &m_posX, &m_posY, m_sizeX, m_sizeY, m_destinationX, m_destinationY, dt);

		updateVertexBuffer();
	}
	if (m_animation->isAnimated()) // Spritesheet or fading
	{
		m_animation->animateElement(m_vertexList, dt);
		m_isReady = m_animation->fadeElement(m_vertexList, dt);
		updateVertexBuffer();
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
	//if (m_vertexList[0].normX)
	if (m_isDrawn)
	{
		UINT32 vertexSize = sizeof(vertex);
		UINT32 offset = 0;

		DX::getInstance()->getDeviceContext()->PSSetShaderResources(0, 1, &m_elementSRV);
		DX::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
		DX::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX::getInstance()->getDeviceContext()->Draw(6, 0);
	}
}

float UI_Element::getPosX()
{
	return m_posX;
}

float UI_Element::getPosY()
{
	return m_posY;
}

void UI_Element::setPos(float posX, float posY, float scale)
{
	if (m_posX != posX || m_posY != posY || scale != 1.0f) // Only update if a change is made
	{
		m_posX = posX;
		m_posY = posY;

		float left, right, top, bottom;
		left = m_posX - m_sizeX * scale / 2.0f;
		right = left + m_sizeX * scale;
		top = m_posY + m_sizeY * scale / 2.0f;
		bottom = top - m_sizeY * scale;

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

		D3D11_MAPPED_SUBRESOURCE mappedMemory;
		DX::getInstance()->getDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
		memcpy(mappedMemory.pData, m_vertexList, sizeof(vertex) * 6);
		DX::getInstance()->getDeviceContext()->Unmap(m_vertexBuffer, 0);
	}
}

void UI_Element::setScale(float scale)
{
	//XMMATRIX scaleM = XMMatrixScaling(scale, scale, scale);
	float posX = m_posX;
	float posY = m_posY;

	setPos(0.0f, 0.0f);

	XMVECTOR temp;

	for (int i = 0; i < 6; i++)
	{
		temp = XMVectorScale(XMVectorSet(m_vertexList[i].posX, m_vertexList[i].posY, m_vertexList[i].posZ, 1.0f), scale);
		//temp = XMVectorSet(m_vertexList[i].posX, m_vertexList[i].posY, m_vertexList[i].posZ, 1.0f) * scaleM;
		m_vertexList[i].posX = temp.m128_f32[0];
		m_vertexList[i].posY = temp.m128_f32[1];
		m_vertexList[i].posZ = temp.m128_f32[2];
	}
	
	setPos(posX, posY, scale);
}

void UI_Element::setDestinationX(float deltaX, float speed, float acceleration, float delay, float rest)
{
	m_destinationX += deltaX;
	m_animation->setAnimationData(speed, acceleration, delay, rest);
}

void UI_Element::setDestinationY(float deltaY, float speed, float acceleration, float delay, float rest)
{
	m_destinationY += deltaY;
	m_animation->setAnimationData(speed, acceleration, delay, rest);
}

void UI_Element::fadeOut(float fadeTime, float delay)
{
	if (fadeTime != 0.0f || delay != 0.0f)
		m_animation->setFadeOut(fadeTime, delay);
	else
	{
		for (int i = 0; i < 6; i++)
		{
			m_vertexList[i].normX = 0.0f;
			m_alpha = 0.0f;
			m_animation->setFadeIn(fadeTime, delay);
			m_animation->setFadeOut(fadeTime, delay);

		}
	}
}

void UI_Element::fadeIn(float fadeTime, float delay)
{
	m_animation->setFadeIn(fadeTime, delay);
}

void UI_Element::setAnimated(bool isAnimated)
{
	m_animation->setAnimated(isAnimated);
}

float UI_Element::getAlpha()
{
	return m_vertexList[0].normX;
}

void UI_Element::setDrawn(bool isDrawn)
{
	m_isDrawn = isDrawn;
}

void UI_Element::setReady(bool isReady)
{
	m_isReady = isReady;
}

bool UI_Element::isDrawn()
{
	return m_isDrawn;
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

ConstantBuffer* UI_Element::getConstantBuffer()
{
	return m_animation->getConstantBuffer();
}
