#include "Robot.h"

Robot::Robot(int playerId)
{
	m_playerId = playerId;
	m_health = 100;
	m_velocity = 3.0f;
	m_currentRotation = 0.0;
	m_colour = XMVectorSet(1,0,0,0);
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	m_score = 0;
	Weapon* pistol = new Weapon(RIFLE);
	m_weapons.push_back(pistol);

	// TODO add init
	//Resource m_resource;
}

void Robot::setPlayerId(int playerId)
{
	m_playerId = playerId;
}

int Robot::getPlayerId()
{
	return m_playerId;
}

void Robot::damagePlayer(int damage)
{
	m_health -= damage;
}

int Robot::getHealth()
{
	return m_health;
}

void Robot::setVelocity(float velocity)
{
	m_velocity = velocity;
}

float Robot::getVelocity()
{
	return m_velocity;
}

void Robot::setCurrentRot(float deg)
{
	m_currentRotation = deg;
}

float Robot::getCurrentRot()
{
	return m_currentRotation;
}

void Robot::setColour(XMVECTOR colour)
{
	m_colour = colour;
}

XMVECTOR Robot::getColour()
{
	return m_colour;
}

void Robot::changeWeapon(int side)
{

	if (m_weapons.size() > 2) 
	{
		m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		if (m_currentWeapon[side] == m_currentWeapon[(side + 1) % 2])
		{
			m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		}
	}
}

int Robot::getCurrentWeapon(int side)
{
	return m_currentWeapon[side];
}

std::vector<Weapon*> Robot::getWeapons()
{
	return m_weapons;
}

void Robot::addScore(int score)
{
	m_score += score;
}

void Robot::resetScore()
{
	m_score = 0;
}

void Robot::addWeapon(int type)
{
	Weapon* weapon = new Weapon();
	m_weapons.push_back(weapon);
	if (m_currentWeapon[LEFT] == -1)
		m_currentWeapon[LEFT] = 1;
}

bool Robot::upgradeWeapon(int type)
{
	for (int i = 0; i < m_weapons.size(); i++)
	{
		if (m_weapons[i]->getType() == type)
		{
			m_weapons[i]->upgrade();
			return true;
		}
	}
	addWeapon(type);
	return  false;
}

void Robot::release()
{
	// TODO realease resource?
	for (int i = 0; i < m_weapons.size(); i++)
	{
		delete m_weapons[i];
	}
}
