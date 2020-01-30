#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "DX.h"


using namespace DirectX;

enum objectType {
	drone = 0,
	weapon = 1,
	robot = 2,
	node = 3,
	projectile = 4,
	resource = 5
};

struct renderData {
	XMVECTOR pos;
	XMVECTOR rotation;
	XMVECTOR scale;
};


class GameObject
	{
private:
	XMVECTOR m_position;
	XMVECTOR m_rotation;
	XMVECTOR m_scale;

	

public:
	GameObject();
	~GameObject();


	void update();
	void draw();

	// Translation
	void setPosition(float x, float y, float z); // Absolute
	void setPosition(XMVECTOR pos);
	void move(float dX, float dY, float dZ); // Relative
	void move(XMVECTOR dPos);

	// Rotation
	void setRotation(XMVECTOR rotation); // Absolute
	void setRotation(float vx, float vy, float vz, float rotDeg);
	void rotate(XMVECTOR dRotation); // Relative
	void rotate(float vx, float vy, float vz, float dRotDeg);

	// Scale
	void setScale(XMVECTOR scale); // Absolute
	void setScale(float xScale, float yScale, float zScale);
	void scale(XMVECTOR scale); // Relative
	void scale(float xScale, float yScale, float zScale);

	XMVECTOR getPosition();
};