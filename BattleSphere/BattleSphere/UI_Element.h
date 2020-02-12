#pragma once

#include "DX.h"
#include "StructsAndEnums.h"
#include "ConstantBuffer.h"
#include <WICTextureLoader.h>
#include <string>
#include <wchar.h>

class UI_Element
{
private:
	float m_posX, m_posY;
	float m_sizeX, m_sizeY;

	ID3D11ShaderResourceView* m_elementSRV;
	ID3D11Texture2D* m_texture;
	ConstantBuffer* m_constantBuffer;
	vertex m_vertexList[6];

	void initializeResources(std::wstring fileName);

public:
	UI_Element(std::wstring fileName, float posX = 0.0f, float posY = 0.0f, float sizeX = 0.0f, float sizeY = 0.0f);
	~UI_Element();

	void updateConstantBuffer();
	void draw();
};

