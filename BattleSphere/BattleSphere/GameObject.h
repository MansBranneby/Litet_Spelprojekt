#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "DX.h"
//#include "Model.h"

using namespace DirectX;

class GameObject
	{
private:
	XMVECTOR m_pos;
	//Model m_model;
public:
	GameObject();

	void update();
	void draw();

	void setPosition(XMVECTOR pos);
	XMVECTOR getPosition();
};