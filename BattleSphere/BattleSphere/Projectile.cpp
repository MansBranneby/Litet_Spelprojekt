#include "Projectile.h"

Projectile::Projectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, float damage, float blastRange, int owner)
{
	setPosition(pos);
	setRotation(rot);
	m_direction = dir;
	m_type = type;
	m_damage = damage;
	m_velocity = 0.0f;
	m_owner = owner;
	m_explode = false;
	m_explodeTime = 0;
	m_material.emission = colour;

	m_material.emission.m128_f32[3] = 1;
	m_blastRange = blastRange;

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
	else if (type == ENERGY)
	{
		m_velocity = 20.0f;
		setScale(2.0f, 2.0f, 2.0f);
		m_material.emission = XMVectorSet(0, 0, 0, -1);
		m_material.specular = XMVectorSet(0, 0, 0, -1);
		m_material.diffuse = colour;
		m_material.emission.m128_f32[3] = 0.2f;
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

float Projectile::getBlastRange()
{
	return m_blastRange;
}

bool Projectile::isExploding()
{
	return m_explode;
}

void Projectile::explode()
{
	m_explode = true;
	m_velocity = 0;
	Sound::getInstance()->play(soundEffect::e_explosion, getPosition(), 0.3f, 0.0f, 0.0f);
}

bool Projectile::move(float dt)
{
	
	if (m_explode)
	{
		m_explodeTime += dt;
		if(m_explodeTime < ENERGY_EXPLODE_TIME)
		{
			float scale = 2.0f + sin(m_explodeTime * XM_PI / (ENERGY_EXPLODE_TIME * 2)) * m_blastRange;
			setScale(scale, scale, scale);
		}
		else
		{
			m_material.emission.m128_f32[3] = 0.2f * (1 - ((m_explodeTime - ENERGY_EXPLODE_TIME) / (ENERGY_EXPLODE_FADETIME)));
			if(m_material.emission.m128_f32[3] <= 0)
			{
				m_explode = true;
				setScale(0, 0, 0);
				return true;
			}
		}
		
		
	}
	else {
		GameObject::move(m_direction * dt * m_velocity);
		
	}
	return false;
}
