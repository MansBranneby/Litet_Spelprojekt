#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Resource.h"

#define LEFT 0
#define RIGHT 1

using namespace DirectX;

class Robot : public GameObject
{
private:
	int m_playerId;
	int m_health;
	float m_velocity;
	float m_currentRotation;
	std::vector<Weapon*> m_weapons;
	int m_currentWeapon[2];
	int m_resource;
	int m_score;
	float m_time;
	bool m_ready;

	void addWeapon(int type);
public:
	Robot(int playerId);

	void setPlayerId(int playerId);
	int getPlayerId();
	bool damagePlayer(int damage, XMVECTOR projDir);
	void setHealth(int health);
	int getHealth();
	void setVelocity(float velocity);
	float getVelocity();
	void setCurrentRot(float deg);
	float getCurrentRot();
	bool isReady(float dt);

	void useWeapon(int side, float dt);
	void changeWeapon(int side);
	int getCurrentWeapon(int side);
	std::vector<Weapon*> getWeapons();
	bool upgradeWeapon(int type);

	void addScore(int score);
	void resetScore();

	void setResourceIndex(int index);
	int getResourceIndex();
	void removeResource();

	void update(float dt);
	void move(XMVECTOR dPos);

	void release();
};