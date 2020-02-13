#pragma once

#include "DX.h"
#include "StructsAndEnums.h"
#include "ConstantBuffer.h"
#include <string>
#include <wchar.h>
// DirectXTK
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

class UI_Element
{
private:
	float m_posX, m_posY;
	float m_sizeX, m_sizeY;
	float m_screenWidth, m_screenHeight;

	ID3D11ShaderResourceView* m_elementSRV;
	//ID3D11Texture2D* m_texture;
	ID3D11Buffer* m_vertexBuffer;
	vertex m_vertexList[6];

	void initializeResources(std::wstring fileName);

public:
	UI_Element(std::wstring fileName, float posX = 0.0f, float posY = 0.0f, float sizeX = 0.0f, float sizeY = 0.0f);
	~UI_Element();

	void updateElement(float posX, float posY);
	void updateVertexBuffer();
	void draw();
};

