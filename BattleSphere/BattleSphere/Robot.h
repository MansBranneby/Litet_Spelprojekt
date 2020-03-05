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
	XMVECTOR m_colour;
	float m_health;
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
	unsigned int m_positionHistoryPtr;	// Pointer to last inserted position
	// Velocity
	DirectX::XMVECTOR m_vel;

	void addWeapon(int type);
public:
	Robot(int playerId);

	void setPlayerId(int playerId);
	int getPlayerId();
	bool damagePlayer(float damage, XMVECTOR projDir, int projIndex, bool playSound = true);
	void setHealth(float health);
	float getHealth();
	void setVelocity(float velocity);
	float getVelocity();
	void setCurrentRot(float deg);
	float getCurrentRot();
	bool isReady(float dt);
	void setColour(float x, float y, float z);

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

	int m_lightIndex;

	void update(float dt);
	void move(XMVECTOR dPos);

	// History of positions
	void storePositionInHistory(DirectX::XMVECTOR position);
	XMVECTOR getPreviousPosition() const;

	// Velocity for collision
	void setVel(DirectX::XMVECTOR vel);
	DirectX::XMVECTOR getVel() const;

	void release();
};