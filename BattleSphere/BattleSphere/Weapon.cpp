#include "Weapon.h"

Weapon::Weapon(int type)
{
	m_relativePos = XMVectorSet(1.9f, 1.4f, 0.2f, 0.0f);
	m_sniperLine[0] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_sniperLine[1] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_damage = 0;
	m_type = type;
	m_recoil = 0.0f;
	m_cooldown = 0.0f;
	m_duration = 0.0f;
	m_speed = 1.0f;
	m_cdTime = 0.0f;
	m_range = 10.0f;
	m_spinPerSec = 0.0f;
	m_maxSpinPerSec = 0.0f;
	m_scale = 1.0f;
	m_ready = true;
	m_currentRecoil = 0.0f;
	m_currentSpeed = 1.0f;
	m_currentDefense = 1.0f;
	m_defense = 1.0f;
	m_spinning = false;

	if (type == RIFLE)
	{
		m_damage = 8;
		m_recoil = 0.4f;
		m_cooldown = 0.2f;

		setScale(0.8f, 0.8f, 0.8f);
	}
	else if (type == MOVEMENT)
	{
		m_cooldown = 10.0f;
		m_duration = 4.0f;
		m_speed = 1.5f;
		setScale(0.1f, 0.8f, 0.1f);
	}
	else if (type == SHIELD)
	{
		m_cooldown = 10.0f;
		m_duration = 5.0f;
		m_defense = 0.0f;
		setScale(0.8f, 0.8f, 0.1f);
	}
	else if (type == DASH)
	{
		m_cooldown = 1.5f;
		m_duration = 0.15f;
		m_speed = 5.0f;
		setScale(0.2f, 1.6f, 0.2f);
	}
	else if (type == REFLECT)
	{
		m_cooldown = 12.0f;
		m_duration = 3.0f;
		m_defense = 0.0f;
		setScale(1.8f, 1.8f, 0.2f);
	}
	else if (type == SNIPER)
	{
		m_damage = 30;
		m_cooldown = 1.8f;

		setScale(0.1f, 0.1f, 0.8f);
	}
	else if (type == BEYBLADE)
	{
		m_cooldown = 0.0f;
		m_speed = 1000;
		m_damage = 0.08f;
		m_range = 4.123295f; // Blade range
		m_scale = 1.0f;
		m_spinPerSec = 0.0f;
		m_maxSpinPerSec = 700.0f;
		m_relativePos = XMVectorSet(0.0f, -0.5f, 0.0f, 0.0f);
		setEmission(0.5f, 0.5f, 0.5f, -1);
	}
	else
	{
		m_damage = 5;
		m_recoil = 0.1f;
		m_cooldown = 0.5f;

		setScale(0.35f, 0.35f, 0.55f);
	}

	m_currentRecoil = m_recoil / 2.0f;
}

int Weapon::getType()
{
	return m_type;
}

float Weapon::getDamage()
{
	return m_damage;
}

float Weapon::getSpinPerSec()
{
	return m_spinPerSec;
}

float Weapon::getSpinTime()
{
	return 1.0f / (m_spinPerSec * 4.0f / 360.0f + 0.01f);
}

float Weapon::getSpinDPS()
{
	return m_spinPerSec * m_damage;
}

float Weapon::getRange()
{
	return m_range * m_scale + +1.54710078f; // Blade range + robot range
}

float Weapon::getRecoil()
{
	return m_recoil;
}

float Weapon::getSpeed()
{
	return m_currentSpeed;
}

float Weapon::getDefense(int robotId, XMVECTOR projDir, XMVECTOR robotPos, XMVECTOR robotColour, float robotRot, int& projIndex)
{
	if (m_type == DASH && m_cdTime < m_duration + 0.1f && !m_ready)
	{
		return 0.0f;
	}
	else if (m_type == REFLECT && m_cdTime < m_duration && !m_ready && projIndex != -1)
	{
		Sound::getInstance()->play(soundEffect::e_reflect, robotPos, 0.3f);
		ProjectileBank::getInstance()->changeDirection(projIndex, robotPos, robotColour, robotId);
		projIndex = -1;
	}
	else if (m_type == SHIELD)
	{
		float rotInRad = XMConvertToRadians(robotRot);

		XMVECTOR robotDir =
			XMVector3Rotate(
				XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
				XMVectorSet(0, (float)sin(rotInRad / 2), 0, (float)cos(rotInRad / 2))
			);
		if (XMVectorGetX(XMVector3Dot(projDir, robotDir)) > 0.0f)
			return 1.0f;
	}
	return m_currentDefense;
}

void Weapon::getSniperLine(XMVECTOR& start, XMVECTOR& end)
{
	start = m_sniperLine[0];
	end = m_sniperLine[1];
}

bool Weapon::getActive()
{
	return m_cdTime < m_duration && !m_ready;
}

bool Weapon::getReady()
{
	return m_ready;
}

void Weapon::upgrade()
{
	if (m_type == RIFLE)
	{
		m_damage += 2;
		m_recoil -= 0.1f;
		m_cooldown -= 0.03f;
		if (m_damage > 20)
			m_damage = 20;
		if (m_cooldown < 0.05f)
			m_cooldown = 0.05f;
	}
	else if (m_type == MOVEMENT)
	{
		m_cooldown -= 0.5f;
		m_duration += 1.0f;
		m_speed += 0.2f;
		if (m_cooldown < 6.0f)
			m_cooldown = 6.0f;
		if (m_duration > 8.0)
			m_duration = 8.0f;
	}
	else if (m_type == SHIELD)
	{
		m_cooldown -= 0.5f;
		m_duration += 0.5f;
		if (m_cooldown < 8.0f)
			m_cooldown = 8.0f;
		if (m_duration > 8.0)
			m_duration = 8.0f;
	}
	else if (m_type == DASH)
	{
		m_cooldown -= 0.1f;
		if (m_cooldown < 1.0f)
			m_cooldown = 1.0f;
	}
	else if (m_type == REFLECT)
	{
		m_cooldown -= 1.0f;
		m_duration += 0.5f;
		if (m_cooldown < 8.0f)
			m_cooldown = 8.0f;
		if (m_duration > 6.0)
			m_duration = 6.0f;
	}
	else if (m_type == SNIPER)
	{
		m_cooldown -= 0.2f;
		m_damage += 5;
		if (m_cooldown < 1.0f)
			m_cooldown = 1.0f;
		if (m_damage > 50)
			m_duration = 50;
	}
	else if (m_type == BEYBLADE)
	{
		if (m_damage < 0.2f)
			m_damage += 0.02f;
		if (m_scale < 2.5f) 
			m_scale += 0.5f;
		if (m_maxSpinPerSec < 2000)
			m_maxSpinPerSec += 200;
		setScale(m_scale, 1.0f, m_scale);
	}
	else
	{
		m_damage += 2;
		m_recoil -= 0.1f;
		m_cooldown -= 0.05f;
		if (m_damage > 15)
			m_damage = 15;
		if (m_cooldown < 0.1f)
			m_cooldown = 0.1f;
	}

	/*
	if (m_damage > 25.0f) m_damage = 25.0f;
	if (m_recoil < 0.0f) m_recoil = 0.0f;
	if (m_cooldown < 2.0f) m_cooldown = 2.0f;
	if (m_duration > 8.0) m_duration = 8.0f;
	*/
}

bool Weapon::shoot(int robotId, XMVECTOR robotPos, XMVECTOR robotColour, float rot, int side, float dt)
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

		ProjectileBank::getInstance()->addProjectile(projPos, robotColour, projRot, projDir, m_type, (float)m_damage, robotId);
		if (m_type == PISTOL)
			Sound::getInstance()->play(soundEffect::e_pistol, projPos, 0.3f, 0.0f, 0.0f);
		if (m_type == RIFLE)
			Sound::getInstance()->play(soundEffect::e_rifle, projPos, 0.3f, -0.5f, 0.0f);

		return true;
	}
	else if (m_type == SNIPER && m_ready)
	{
		m_ready = false;
		return true;
	}

	return false;
}

bool Weapon::speedUp(XMVECTOR robotPos)
{
	if ((m_type == MOVEMENT || m_type == DASH) && m_ready)
	{
		if (m_type == DASH)
			Sound::getInstance()->play(soundEffect::e_dash, robotPos, 0.4f);
		if (m_type == MOVEMENT)
			Sound::getInstance()->play(soundEffect::e_movement, robotPos, 0.4f);
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

bool Weapon::reflect()
{
	if ((m_type == REFLECT) && m_ready)
	{
		m_ready = false;
		return true;
	}
	return false;
}

void Weapon::updateSniperShot(XMVECTOR robotPos, XMVECTOR robotColour, float rot, int side, float dt, QuadtreeNode* qtn, XMVECTOR& start, XMVECTOR& end)
{
	
	float rotInRad = XMConvertToRadians(rot);

	start = XMVector3Rotate(
		XMVectorSet(
			XMVectorGetX(m_relativePos),
			XMVectorGetY(m_relativePos),
			0.0f, 0.0f),
		XMVectorSet(0, (float)sin(rotInRad / 2), 0, (float)cos(rotInRad / 2))
	) + robotPos;

	XMVECTOR dir;
	if (side)
		dir = XMVector3Normalize(XMVector3Cross((start - robotPos), XMVectorSet(0, 1, 0, 0)));
	else
		dir = XMVector3Normalize(XMVector3Cross(XMVectorSet(0, 1, 0, 0), (start - robotPos)));

	XMFLOAT2 start2, dir2;
	start2.x = XMVectorGetX(start);
	start2.y = XMVectorGetZ(start);
	dir2.x = XMVectorGetX(dir);
	dir2.y = XMVectorGetZ(dir);
	float t = qtn->testCollisionRay(start2, dir2);
	if (t == -1.0f)
		t = 400.0f;
	end = start + dir * t;
	//start = XMVectorSetY(start, 2.0f);
	//end = XMVectorSetY(end, 2.0f);
	m_sniperLine[0] = start;
	m_sniperLine[1] = end;
}

bool Weapon::spin(float dt)
{
	if ((m_type == BEYBLADE))
	{
		m_spinning = true;
		return true;
	}
	return false;
}

bool Weapon::updateTime(float dt, XMVECTOR robotPos)
{
	if (m_type != BEYBLADE)
	{
		if (!m_ready)
		{
			m_cdTime += dt;
			if (m_cdTime < m_duration) // Using ability
			{
				m_currentSpeed = m_speed;
				m_currentDefense = m_defense;
				if (m_type == SHIELD || m_type == REFLECT)
					Sound::getInstance()->play(soundAmbient::e_shield, robotPos, 0.9f);
				return true;
			}
			else if (m_cdTime > m_duration + m_cooldown) // Ability ready again
			{
				m_cdTime = 0.0f;
				m_ready = true;
			}
			else
			{
				if (m_type == SHIELD || m_type == REFLECT)
					Sound::getInstance()->stop(soundAmbient::e_shield);
			}
		}
		m_currentSpeed = 1.0f;
		m_currentDefense = 1.0f;

		if (m_type == BEYBLADE)
		{

		}
		return false;
	}
	else
	{
		if (m_spinning)
		{
			m_spinPerSec += m_maxSpinPerSec / 3.0f * dt;
			if (m_spinPerSec > m_maxSpinPerSec)
				m_spinPerSec = m_maxSpinPerSec;
			rotate(0, 1, 0, -m_spinPerSec * dt);
		}
		else
		{
			m_spinPerSec -= m_maxSpinPerSec / 2.0f * dt;
			if (m_spinPerSec < 0)
				m_spinPerSec = 0.0f;
			else
			rotate(0, 1, 0, -m_spinPerSec * dt);
		}

		//Sound::getInstance()->play(soundAmbient::e_melee, robotPos, 0.1f * m_spinPerSec/m_maxSpinPerSec, 0.7f, 1.0f);
		m_spinning = false;
		return false;
	}
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
