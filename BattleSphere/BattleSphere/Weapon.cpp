#include "Weapon.h"

Weapon::Weapon(int type)
{
	m_relativePos = XMVectorSet(1.4f, 0.4f, 0.2f, 0.0f);
	m_damage = 0;
	m_type = type;
	m_recoil = 0.0f;
	m_cooldown = 0.0f;
	m_duration = 0.0f;
	m_speed = 0.0f;
	m_cdTime = 0.0f;
	m_ready = true;

	//TODO change this yo
	loadFromFile("1mesh1mat");

	if (type == RIFLE)
	{
		m_damage = 10;
		m_recoil = 0.4f;
		m_cooldown = 0.2f;

		scale(0.2f, 0.2f, 0.8f);
	}
	else if (type == MOVEMENT)
	{
		m_cooldown = 5.0f;
		m_duration = 5.0f;
		m_speed = 2.0f;
	}
	else if (type == SHIELD)
	{
		m_cooldown = 5.0f;
		m_duration = 5.0f;
	}
	else
	{
		m_damage = 5;
		m_recoil = 0.1f;
		m_cooldown = 0.5f;

		scale(0.2f, 0.2f, 0.8f);
	}
}

int Weapon::getType()
{
	return m_type;
}

int Weapon::getDamage()
{
	return m_damage;
}

float Weapon::getRecoil()
{
	return m_recoil;
}

float Weapon::getSpeed()
{
	return m_speed;
}

void Weapon::upgrade()
{
	if (m_type == RIFLE)
	{
		m_damage += 2;
		m_recoil -= 0.1f;
		m_cooldown -= 0.05f;
	}
	else if (m_type == MOVEMENT)
	{
		m_cooldown -= 0.5f;
		m_duration += 1.0f;
		m_speed += 0.5f;
	}
	else if (m_type == SHIELD)
	{
		m_cooldown -= 0.5f;
		m_duration += 1.0f;
	}
	
	/*
	if (m_damage > 25.0f) m_damage = 25.0f;
	if (m_recoil < 0.0f) m_recoil = 0.0f;
	if (m_cooldown < 2.0f) m_recoil = 2.0f;
	if (m_duration > 8.0) m_duration = 8.0f;
	*/
}

bool Weapon::shoot()
{
	if ((m_type == PISTOL || m_type == RIFLE) && m_ready)
	{
		m_ready = false;
		return true;
	}
	return false;
}

bool Weapon::speedUp()
{
	if ((m_type == MOVEMENT) && m_ready)
	{
		m_ready = false;
		return true;
	}
	return false;
}

bool Weapon::shield()
{
	if ((m_type == SHIELD) && m_ready)
	{
		m_ready = false;
		return true;
	}
	return false;
}

bool Weapon::updateTime(float dt)
{
	if (!m_ready)
	{
		m_cdTime += dt;
		if (m_cdTime < m_duration)
		{
			return true;
		}
		if (m_cdTime > m_duration + m_cooldown)
		{
			m_cdTime = 0.0f;
			m_ready = true;
		}
	}
	return false;
}

XMVECTOR Weapon::getRelativePos()
{
	return m_relativePos;
}

void Weapon::release()
{
}
