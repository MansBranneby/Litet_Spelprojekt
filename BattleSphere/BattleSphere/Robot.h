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

	// Position history of robot
	XMVECTOR* m_positionHistory;		// Array of positions
	unsigned int m_positionHistorySize; // Current size
	unsigned int m_positionHistoryCap;  // Cap
	unsigned int m_positionHistoryPtr;	// Point to last inserted position

	void addWeapon(int type);
public:
	Robot(int playerId);

	void setPlayerId(int playerId);
	int getPlayerId();
	void damagePlayer(int damage);
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

	// History of positions
	void storePositionInHistory(DirectX::XMVECTOR position);
	XMVECTOR getPreviousPosition() const;

	void release();
};