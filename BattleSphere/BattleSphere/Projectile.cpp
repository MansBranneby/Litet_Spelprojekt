#include "Projectile.h"

Projectile::Projectile(XMVECTOR pos, XMVECTOR rot, XMVECTOR dir, int type, int damage)
{
	setPosition(pos);
	setRotation(rot);
	m_direction = dir;
	m_type = type;
	m_damage = damage;
	m_velocity = 0.0f;

	m_material.emission = XMVectorSet(0.0f, 1.0f, 0.0f, -1);

	if (type == PISTOL)
	{
		m_velocity = 40.0f;
		//setScale(0.5f, 0.5f, 0.8f);
		setScale(0.15f, 0.15f, 0.3f);
	}
	else if (type == RIFLE)
	{
		m_velocity = 50.0f;
		//setScale(0.5f, 0.5f, 0.8f);
		setScale(0.15f, 0.15f, 0.3f);
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
