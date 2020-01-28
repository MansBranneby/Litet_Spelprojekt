#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "DX.h"
#include "Model.h"


using namespace DirectX;

class GameObject
	{
private:
	XMVECTOR m_pos;

	int m_nrOfModels;
	Model* m_models;
	

public:
	GameObject();
	~GameObject();
	void loadFromFile(std::string filename); // Without .nyp file extension
	void update();
	void draw();

	void setPosition(XMVECTOR pos);
	XMVECTOR getPosition();
};