#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"
#include "Weapon.h"
//#include "Resource.h"

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
	XMVECTOR m_colour;
	std::vector<Weapon*> m_weapons;
	int m_currentWeapon[2];
	//Resource m_resource; pekare?
	int m_score;

	void addWeapon(int type);
public:
	Robot(int playerId);

	void setPlayerId(int playerId);
	int getPlayerId();
	void damagePlayer(int damage);
	int getHealth();
	void setVelocity(float velocity);
	float getVelocity();
	void setCurrentRot(float deg);
	float getCurrentRot();
	void setColour(XMVECTOR colour);
	XMVECTOR getColour();
	void changeWeapon(int side);
	int getCurrentWeapon(int side);
	std::vector<Weapon*> getWeapons();
	void addScore(int score);
	void resetScore();

	bool upgradeWeapon(int type);
	//bool checkResource(Resource resource);
	//void removeResource();

	void release();
};