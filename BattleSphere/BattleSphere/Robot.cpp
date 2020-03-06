#include "Robot.h"

Robot::Robot(int playerId)
{
	m_playerId = playerId;
	m_health = 100;
	m_velocity = 20.0f;
	m_currentRotation = 0.0;
	m_currentWeapon[LEFT] = -1;
	m_currentWeapon[RIGHT] = 0;
	m_score = 0;
	m_resource = -1;
	Weapon* pistol = new Weapon(PISTOL);
	m_weapons.push_back(pistol);
	m_ready = true;
	m_time = 0;
	m_material.ambient = XMVectorSet(0.5, 0.5, 0.5, -1);
	m_material.diffuse = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_material.emission = XMVectorSet(0.0, 0.0, 0.0, -1);
	m_colour = XMVectorSet(1.0, 1.0, 1.0, -1);
	//setScale(2.2f, 2.2f, 2.2f);
	// Raise player
	setPosition(XMVECTOR{ 10.0f, 1.0f, 0.0f });
	m_lightIndex = Lights::getInstance()->addPointLight(0, 0, 0, 10, 1, 1, 1, 10);
	// Position history
	m_positionHistorySize = 0; 
	m_positionHistoryCap = 100;
	m_positionHistoryPtr = 0;	
	m_positionHistory = new DirectX::XMVECTOR[m_positionHistoryCap];
	m_positionHistory[m_positionHistoryCap - 1] = getPosition();
}

void Robot::setPlayerId(int playerId)
{
	m_playerId = playerId;
}

int Robot::getPlayerId()
{
	return m_playerId;
}

XMVECTOR Robot::getColour()
{
	return m_colour;
}


bool Robot::damagePlayer(int damage, XMVECTOR projDir, int projIndex)
{
	float dmg = (float)damage;
	if (m_currentWeapon[RIGHT] != -1)
		dmg *= m_weapons[m_currentWeapon[RIGHT]]->getDefense(m_playerId, projDir, getPosition(), m_colour, m_currentRotation, projIndex);
	if (m_currentWeapon[LEFT] != -1)
		dmg *= m_weapons[m_currentWeapon[LEFT]]->getDefense(m_playerId, projDir, getPosition(), m_colour, m_currentRotation, projIndex);

	if (projIndex != -1)
		ProjectileBank::getInstance()->removeProjectile(projIndex);

	if (dmg != 0.0f)
	{
		Sound::getInstance()->play(soundEffect::e_damage, getPosition(), 0.3f);
		m_health -= (int)floorf(dmg);
		if (m_health < 0)
		{
			m_health = 0;
			setDrawn(false);
		}
		m_material.emission = m_colour * (float)m_health / 100.0f;
		removeResource();
		return true;
	}
	else
		Sound::getInstance()->play(soundEffect::e_impact, getPosition(), 0.05f);
	return false;
}

void Robot::setHealth(int health)
{
	setDrawn(true);
	m_health = health;
	m_material.emission = m_colour;
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
	float velocity = m_velocity;
	if (m_currentWeapon[RIGHT] != -1)
		velocity *= m_weapons[m_currentWeapon[RIGHT]]->getSpeed();
	if (m_currentWeapon[LEFT] != -1)
		velocity *= m_weapons[m_currentWeapon[LEFT]]->getSpeed();
	return velocity;
}

void Robot::setCurrentRot(float deg)
{
	m_currentRotation = deg;
}

float Robot::getCurrentRot()
{
	return m_currentRotation;
}

bool Robot::isReady(float dt)
{
	if (!m_ready)
	{
		m_time += dt;
		if (m_time > 0.5f)
		{
			m_time = 0;
			m_ready = true;
		}
		return false;
	}
	return true;
}

void Robot::setColour(float x, float y, float z)
{
	m_material.emission = m_colour = XMVectorSet(x, y, z, -1);
}

void Robot::useWeapon(int side, float dt)
{
	if (m_currentWeapon[side] != -1)
	{
		m_weapons[m_currentWeapon[side]]->shoot(m_playerId, getPosition(), m_colour, m_currentRotation, side, dt);
		m_weapons[m_currentWeapon[side]]->speedUp(getPosition());
		m_weapons[m_currentWeapon[side]]->shield();
		m_weapons[m_currentWeapon[side]]->reflect();
	}
}

void Robot::changeWeapon(int side)
{
	if (m_weapons.size() > 2) 
	{
		m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		if (m_currentWeapon[side] == m_currentWeapon[(side + 1) % 2])
			m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		
		if (side == RIGHT)
			m_weapons[m_currentWeapon[RIGHT]]->setRelativePos(XMVectorSet(1.9f, 1.4f, 0.2f, 0.0f));
		else
			m_weapons[m_currentWeapon[LEFT]]->setRelativePos(XMVectorSet(-1.9f, 1.4f, 0.2f, 0.0f));
		m_ready = false;
	}
}

int Robot::getCurrentWeapon(int side)
{
	return m_currentWeapon[side];
}

std::vector<Weapon*> Robot::getWeapons()
{
	return m_weapons;
}

void Robot::addScore(int score)
{
	m_score += score;
}

void Robot::resetScore()
{
	m_score = 0;
}

int Robot::getScore() const
{
	return m_score;
}

void Robot::addWeapon(int type)
{
	Weapon* weapon = new Weapon(type);
	if (m_currentWeapon[LEFT] == -1)
	{
		m_currentWeapon[LEFT] = 1;
		weapon->setRelativePos(XMVectorSet(-1.4f, 0.4f, 0.2f, 0.0f));
	}
	m_weapons.push_back(weapon);
}

bool Robot::upgradeWeapon(int type)
{
	for (int i = 0; i < m_weapons.size(); i++)
	{
		if (m_weapons[i]->getType() == type)
		{
			m_weapons[i]->upgrade();
			return true;
		}
	}
	addWeapon(type);
	return  false;
}

void Robot::setResourceIndex(int index)
{
	m_resource = index;
}

int Robot::getResourceIndex()
{
	return m_resource;
}

void Robot::removeResource()
{
	m_resource = -1;
}

void Robot::update(float dt)
{
	GameObject::update();
	XMVECTOR position = GameObject::getPosition();
	Lights::getInstance()->setPosition(m_lightIndex, position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	objectData objectData = GameObject::getData();
	XMVECTOR color = objectData.material.emission;
	Lights::getInstance()->setColor(m_lightIndex, color.m128_f32[0], color.m128_f32[1], color.m128_f32[2]);
	for (int i = 0; i < m_weapons.size(); i++)
	{
		m_weapons[i]->updateTime(dt, getPosition());
	}
}

void Robot::move(XMVECTOR dPos)
{
	GameObject::move(dPos);
	
	m_weapons[m_currentWeapon[RIGHT]]->setPosition(
		m_weapons[m_currentWeapon[RIGHT]]->getRelativePos()
	);

	if (getCurrentWeapon(LEFT) != -1)
	{
		m_weapons[m_currentWeapon[LEFT]]->setPosition(
			m_weapons[m_currentWeapon[LEFT]]->getRelativePos()
		);
	}
}

void Robot::storePositionInHistory(DirectX::XMVECTOR position)
{
	if (m_positionHistorySize < m_positionHistoryCap)
	{
		// size is less than cap
		m_positionHistory[m_positionHistoryPtr] = position; // insert at historyPtr
		m_positionHistoryPtr++;								// step historyPtr
		m_positionHistorySize++;							// increase size
	}
	else
	{
		// size has grown past cap
		m_positionHistoryPtr = 0;							// reset ptr
		m_positionHistorySize = 0;							// reset size
		m_positionHistory[m_positionHistoryPtr] = position; // replace element at ptr 
	}

}

XMVECTOR Robot::getPreviousPosition() const
{
	if (m_positionHistoryPtr == 0)
	{
		return m_positionHistory[m_positionHistoryCap - 1];
	}

	return m_positionHistory[m_positionHistoryPtr - 1];
}

void Robot::setVel(DirectX::XMVECTOR vel)
{
	m_vel = vel;
}

DirectX::XMVECTOR Robot::getVel() const
{
	return m_vel;
}

void Robot::release()
{
	// TODO realease resource?
	for (int i = 0; i < m_weapons.size(); i++)
	{
		delete m_weapons[i];
	}

	delete[] m_positionHistory;
}
