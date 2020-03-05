#include "Projectile.h"

Projectile::Projectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, float damage, int owner)
{
	setPosition(pos);
	setRotation(rot);
	m_direction = dir;
	m_type = type;
	m_damage = damage;
	m_velocity = 0.0f;
	m_owner = owner;

	m_material.emission = colour;

	if (type == PISTOL)
	{
		m_velocity = 40.0f;
		setScale(0.35f, 0.35f, 0.35f);
	}
	else if (type == RIFLE)
	{
		m_velocity = 80.0f;
		setScale(0.35f, 0.35f, 0.35f);
	}
}

void Projectile::setDirection(XMVECTOR relPos, XMVECTOR colour, int owner)
{
	m_material.emission = colour;
	m_owner = owner;

	m_direction = getPosition() - relPos;
	m_direction = XMVectorSetY(m_direction, 0.0f);
	m_direction = XMVector3Normalize(m_direction);
	float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(m_direction, XMVectorSet(0, 0, 1, 0))));
	if (XMVectorGetX(getPosition()) < XMVectorGetX(relPos))
		angle *= -1;
	setRotation(XMVectorSet(0, 1, 0, angle));
}

int Projectile::getOwner()
{
	return m_owner;
}

int Projectile::getType()
{
	return m_type;
}

float Projectile::getDamage()
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
