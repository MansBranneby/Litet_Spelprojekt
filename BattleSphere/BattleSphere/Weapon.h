#pragma once

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "ProjectileBank.h"
#include "StructsAndEnums.h"
#include "Sound.h"

using namespace DirectX;

class Weapon : public GameObject
{
private:
	XMVECTOR m_relativePos;
	float m_damage;
	int m_type;
	float m_recoil;
	float m_currentRecoil;
	float m_speed;
	float m_currentSpeed;
	float m_defense;
	float m_currentDefense;

	bool m_spinning;
	float m_scale;
	float m_range;
	float m_spinPerSec;
	float m_maxSpinPerSec;

	bool m_ready;
	float m_cooldown;
	float m_duration;
	float m_cdTime;

public:
	Weapon(int type = 0);

	void upgrade();

	bool shoot(int robotId, XMVECTOR robotPos, XMVECTOR robotColour, float rot, int side, float dt);
	bool speedUp(XMVECTOR robotPos);
	bool shield();
	bool reflect();
	bool spin(float dt);

	void setRelativePos(XMVECTOR pos);
	XMVECTOR getRelativePos();
	int getType();
	float getDamage();
	float getSpinPerSec();
	float getSpinTime();
	float getSpinDPS();
	float getRange();
	float getRecoil();
	float getSpeed();
	bool getActive();
	float getDefense(int robotId, XMVECTOR projDir, XMVECTOR robotPos, XMVECTOR robotColour, float robotRot, int& projIndex);

	bool updateTime(float dt, XMVECTOR robotPos);

	void release();
};