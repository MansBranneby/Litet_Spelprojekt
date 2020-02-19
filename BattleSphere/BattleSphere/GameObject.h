#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "DX.h"
#include "StructsAndEnums.h"

using namespace DirectX;




class GameObject
{
private:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
	XMVECTOR m_scale;

	bool m_isDrawn;

public:
	GameObject();
	~GameObject();


	void update();

	// Translation
	void setPosition(float x, float y, float z); // Absolute
	void setPosition(XMVECTOR pos);
	void move(float dX, float dY, float dZ); // Relative current value
	void move(XMVECTOR dPos);
	// Set relative object

	// Rotation
	void setRotation(XMVECTOR rotation); // Absolute
	void setRotation(float vx, float vy, float vz, float rotDeg);
	void rotate(XMVECTOR dRotation); // Relative current value
	void rotate(float vx, float vy, float vz, float dRotDeg);
	// Set relative object

	// Scale
	void setScale(XMVECTOR scale); // Absolute
	void setScale(float xScale, float yScale, float zScale);
	void scale(XMVECTOR scale); // Relative
	void scale(float xScale, float yScale, float zScale);

	XMVECTOR getPosition();
	objectData const getData();

	bool isDrawn();
	void setDrawn(bool isDrawn);
};
