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
	XMVECTOR m_staticRotation;
	XMVECTOR m_rotation;
	XMVECTOR m_scale;

	bool m_isDrawn;

protected:
	material m_material;

public:
	GameObject();
	~GameObject();


	void update();

	// Translation
	void setPosition(float x, float y, float z); // Absolute
	void setPosition(XMVECTOR pos);
	void move(float dX, float dY, float dZ); // Relative current value
	void move(XMVECTOR dPos);

	// Rotation
	void setRotation(XMVECTOR rotation); // Absolute
	void setRotation(float vx, float vy, float vz, float rotDeg);
	void rotate(XMVECTOR dRotation); // Relative current value
	void rotate(float vx, float vy, float vz, float dRotDeg);

	// Scale
	void setScale(XMVECTOR scale); // Absolute
	void setScale(float xScale, float yScale, float zScale);
	void scale(XMVECTOR scale); // Relative
	void scale(float xScale, float yScale, float zScale);

	// Material
	void setAmbiet(float x, float y, float z, float w = -1);
	void setDiffuse(float x, float y, float z, float w = -1);
	void setSpecular(float x, float y, float z, float w = -1);
	void setEmission(float x, float y, float z, float w = -1);

	XMVECTOR getPosition();
	objectData const getData();

	bool isDrawn();
	void setDrawn(bool isDrawn);
};
