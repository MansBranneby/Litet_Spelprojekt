#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"

using namespace DirectX;

#define PISTOL 0
#define RIFLE 1

#define ENERGY_EXPLODE_TIME 0.15f
#define ENERGY_EXPLODE_RADIUS 20.0f
#define ENERGY_EXPLODE_FADETIME 0.5f
class Projectile : public GameObject
{
private:
	bool m_explode;
	float m_explodeTime;
	int m_owner;
	int m_type;
	int m_damage;
	float m_velocity;
	XMVECTOR m_direction;

public:
	Projectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, int damage, int owner);

	void setDirection(XMVECTOR relPos, XMVECTOR colour, int owner);

	int getOwner();
	int getType();
	int getDamage();
	float getVelocity();
	XMVECTOR getDirection();
	bool isExploding();
	void explode();
	bool move(float dt);
};