#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
using namespace DirectX;

#define PISTOL 0
#define RIFLE 1
#define MOVEMENT 2
#define SHIELD 3

class Weapon : public GameObject
{
private:
	XMVECTOR m_relativePos;
	int m_damage;
	int m_type;
	float m_recoil;
	float m_speed;
	float m_cooldown;
	float m_duration;
	float m_cdTime;
	bool m_ready;

public:
	Weapon(int type = 0);

	void upgrade();

	bool shoot();
	bool speedUp();
	bool shield();

	XMVECTOR getRelativePos();
	int getType();
	int getDamage();
	float getRecoil();
	float getSpeed();

	bool updateTime(float dt);

	void release();
};