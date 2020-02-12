#include "Projectile.h"

Projectile::Projectile(XMVECTOR pos, XMVECTOR rot, XMVECTOR dir, int type, int damage)
{
	setPosition(pos);
	setRotation(rot);
	m_direction = dir;
	m_type = type;
	m_damage = damage;
	m_velocity = 0.0f;

	if (type == PISTOL)
	{
		m_velocity = 40.0f;
		setScale(0.08f, 0.08f, 0.2f);
	}
	else if (type == RIFLE)
	{
		m_velocity = 50.0f;
		setScale(0.08f, 0.08f, 0.2f);
	}
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

void Projectile::move(float dt)
{
	GameObject::move(m_direction * dt * m_velocity);
}
