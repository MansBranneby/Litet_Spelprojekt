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

	int m_nrOfModels;
	Model* m_models;
	

public:
	GameObject();
	~GameObject();

	void loadFromFile(std::string filename); // Without .nyp file extension
	void update();
	void draw();

	void setPosition(float x, float y, float z);
	void setPosition(XMVECTOR pos);
	void move(XMVECTOR dPos);
	void move(float dX, float dY, float dZ);
	void rotate(float vx, float vy, float vz, float rotDeg);
	void scale(float xScale, float yScale, float zScale);
};