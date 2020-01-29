#include "Robot.h"

Robot::Robot(int playerId)
{
	m_playerId = 0;
	m_health = 100;
	m_velocity = 3.0f;
	m_colour = XMVectorSet(1,0,0,0);
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = -1;
	m_score = 0;
	// TODO add init
	//std::vector<Weapon> m_weapons;
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
	// TODO remove comment and try the function
	/*
	if (m_weapons.length() > 2) 
	{
		m_currentWeapon[side] = (m_currentWeapon[side] + 1) % m_weapons.length();
		if (m_currentWeapon[side] == m_currentWeapon[(side + 1) % 2])
		{
			m_currentWeapon[side] = (m_currentWeapon[side] + 1) % m_weapons.length();
		}
	}
	*/
}

int Robot::getCurrentWeapon(int side)
{
	return m_currentWeapon[side];
}

void Robot::addScore(int score)
{
	m_score += score;
}

void Robot::resetScore()
{
	m_score = 0;
}

void Robot::release()
{
	// TODO realease weapons/resource?
}
