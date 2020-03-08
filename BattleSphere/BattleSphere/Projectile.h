#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "Sound.h"

using namespace DirectX;

#define ENERGY_EXPLODE_TIME 0.15f
#define ENERGY_EXPLODE_FADETIME 0.5f
class Projectile : public GameObject
{
private:
	bool m_explode;
	float m_explodeTime;
	int m_owner;
	int m_type;
	float m_damage;
	float m_blastRange;
	float m_velocity;
	XMVECTOR m_direction;

public:
	Projectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, float damage, float blastRange, int owner);

	void setDirection(XMVECTOR relPos, XMVECTOR colour, int owner);

	int getOwner();
	int getType();
	float getDamage();
	float getVelocity();
	XMVECTOR getDirection();
	float getBlastRange();
	bool isExploding();
	void explode();
	bool move(float dt);
};