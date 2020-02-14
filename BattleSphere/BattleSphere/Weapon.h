#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "ProjectileBank.h"
#include "StructsAndEnums.h"

using namespace DirectX;

class Weapon : public GameObject
{
private:
	XMVECTOR m_relativePos;
	int m_damage;
	int m_type;
	float m_recoil;
	float m_currentRecoil;
	float m_speed;
	float m_currentSpeed;
	float m_defense;
	float m_currentDefense;

	bool m_ready;
	float m_cooldown;
	float m_duration;
	float m_cdTime;

public:
	Weapon(int type = 0);

	void upgrade();

	bool shoot(XMVECTOR robotPos, float rot, int side, float dt);
	bool speedUp();
	bool shield();

	void setRelativePos(XMVECTOR pos);
	XMVECTOR getRelativePos();
	int getType();
	int getDamage();
	float getRecoil();
	float getSpeed();
	bool getActive();
	float getDefense(XMVECTOR projDir, float robotRot);

	bool updateTime(float dt);

	void release();
};