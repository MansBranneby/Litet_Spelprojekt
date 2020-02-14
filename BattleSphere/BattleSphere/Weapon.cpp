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
	m_currentRecoil = 0.0f;
	m_currentSpeed = 1.0f;

	if (type == RIFLE)
	{
		m_damage = 10;
		m_recoil = 0.4f;
		m_cooldown = 0.2f;

		setScale(0.2f, 0.2f, 0.8f);
	}
	else if (type == MOVEMENT)
	{
		m_cooldown = 5.0f;
		m_duration = 5.0f;
		m_speed = 2.0f;
		setScale(0.1f, 0.8f, 0.1f);
	}
	else if (type == SHIELD)
	{
		m_cooldown = 5.0f;
		m_duration = 5.0f;
		setScale(0.8f, 0.8f, 0.1f);
	}
	else
	{
		m_damage = 5;
		m_recoil = 0.1f;
		m_cooldown = 0.5f;

		setScale(0.15f, 0.15f, 0.6f);
	}

	m_currentRecoil = m_recoil / 2.0f;
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
	return m_currentSpeed;
}

bool Weapon::getActive()
{
	return m_cdTime < m_duration;
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

bool Weapon::shoot(XMVECTOR robotPos, float rot, int side, float dt)
{
	if ((m_type == PISTOL || m_type == RIFLE) && m_ready)
	{
		m_ready = false;

		float rotInRad = XMConvertToRadians(rot);

		XMVECTOR projPos =
			XMVector3Rotate(
				XMVectorSet(
					XMVectorGetX(m_relativePos),
					XMVectorGetY(m_relativePos),
					0.0f, 0.0f),
				XMVectorSet(0, (float)sin(rotInRad / 2), 0, (float)cos(rotInRad / 2))
			) + robotPos;

		XMVECTOR projRot = XMVectorSet(0.0, 1.0, 0.0f, rot);

		/*
		m_currentRecoil += 0.02f;
		if (m_currentRecoil >= m_recoil)
			m_currentRecoil = 0.0f;

		// TODO add recoil here 
		if (m_recoil / 2 <= m_currentRecoil)
			rotate(XMVectorSet(0.0, 1.0, 0.0f, rot));
		else
		{
		}
		*/

		XMVECTOR projDir;
		if (side)
			projDir = XMVector3Cross((projPos - robotPos), XMVectorSet(0, 1, 0, 0));
		else
			projDir = XMVector3Cross(XMVectorSet(0, 1, 0, 0), (projPos - robotPos));

		ProjectileBank::getInstance()->addProjectile(projPos, projRot, projDir, m_type, m_damage);

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
		if (m_cdTime < m_duration) // Using ability
		{
			if (m_type == MOVEMENT)
				m_currentSpeed = m_speed;
			return true;
		}
		if (m_cdTime > m_duration + m_cooldown) // Ability ready again
		{
			m_cdTime = 0.0f;
			m_ready = true;
		}
	}
	if (m_type == MOVEMENT)
		m_currentSpeed = 1.0f;
	return false;
}

void Weapon::setRelativePos(XMVECTOR pos)
{
	m_relativePos = pos;
}

XMVECTOR Weapon::getRelativePos()
{
	return m_relativePos;
}

void Weapon::release()
{
}
