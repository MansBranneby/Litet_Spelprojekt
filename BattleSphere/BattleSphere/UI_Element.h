#pragma once

#include "DX.h"
#include "UI_Animation.h"
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
#include "imgui/imgui.h"

class UI_Element
{
private:
	//uiType m_type;
	UI_Animation* m_animation;

	float m_posX, m_posY;
	float m_destinationX, m_destinationY;
	float m_sizeX, m_sizeY;
	float m_spriteSizeX, m_spriteSizeY;

	float m_alpha;
	bool m_isDrawn;
	bool m_isReady;
	float m_selectionTimer;

	ID3D11ShaderResourceView* m_elementSRV;
	//ID3D11Texture2D* m_texture;
	ID3D11Buffer* m_vertexBuffer;
	vertex m_vertexList[6];

	void initializeResources(std::wstring fileName);

public:
	UI_Element(std::wstring fileName, bool isDrawn, float posX = 0.0f, float posY = 0.0f, float sizeX = 0.0f, float sizeY = 0.0f, float spriteSizeX = 0.0f, float spriteSizeY = 0.0f, u_int nrOfFrames = 0);
	~UI_Element();

	void updateElement(float dt);
	void updateVertexBuffer();
	void draw();

	float getPosX();
	float getPosY();
	void setPos(float posX, float posY);
	void setDestinationX(float deltaX, float speed, float acceleration, float delay, float rest);
	void setDestinationY(float deltaY, float speed, float acceleration, float delay, float rest);
	void fadeOut(float fadeTime, float delay);
	void fadeIn(float fadeTime, float delay);
	void setAnimated(bool isAnimated);
	
	float getAlpha();
	void setDrawn(bool isDrawn);
	void setReady(bool isReady);
	bool isDrawn();
	bool isReady();
	void setSelectionTimer(float time);

	ConstantBuffer* getConstantBuffer();
};

