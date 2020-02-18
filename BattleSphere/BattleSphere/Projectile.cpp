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
		m_velocity = 5.0f;
		//setScale(0.5f, 0.5f, 0.8f);
		setScale(0.15f, 0.15f, 0.3f);
	}
}

void Projectile::setDirection(XMVECTOR relPos)
{
	m_direction = getPosition() - relPos;
	m_direction = XMVectorSetY(m_direction, 0.0f);
	m_direction = XMVector3Normalize(m_direction);
	float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(m_direction, XMVectorSet(0, 0, 1, 0))));
	if (XMVectorGetX(getPosition()) < XMVectorGetX(relPos))
		angle *= -1;
	setRotation(XMVectorSet(0, 1, 0, angle));
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
