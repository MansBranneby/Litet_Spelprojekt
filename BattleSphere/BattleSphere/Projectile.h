#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"

using namespace DirectX;

#define PISTOL 0
#define RIFLE 1

class Projectile : public GameObject
{
private:
	int m_owner;
	int m_type;
	float m_damage;
	float m_velocity;
	XMVECTOR m_direction;

public:
	Projectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, float damage, int owner);

	void setDirection(XMVECTOR relPos, XMVECTOR colour, int owner);

	int getOwner();
	int getType();
	float getDamage();
	float getVelocity();
	XMVECTOR getDirection();

	void move(float dt);
};