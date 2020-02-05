#include "Projectile.h"

Projectile::Projectile(int type, int damage)
{
	m_type = type;
	m_damage = damage;
	m_velocity = 0.0f;
	m_direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	setScale(0.08f, 0.08f, 0.2f);

	if (type == PISTOL)
	{
		m_velocity = 40.0f;
	}
	else if (type == RIFLE)
	{
		m_velocity = 50.0f;
	}
}

void Projectile::setDamage(int damage)
{
	m_damage = damage;
}

void Projectile::setDirection(XMVECTOR direction)
{
	m_direction = XMVector3Normalize(direction) * m_velocity;
}

int Projectile::getType()
{
	return m_type;
}

int Projectile::getDamage()
{
	return m_damage;
}

float Projectile::getVelocity()
{
	return m_velocity;
}

XMVECTOR Projectile::getDirection()
{
	return m_direction;
}
