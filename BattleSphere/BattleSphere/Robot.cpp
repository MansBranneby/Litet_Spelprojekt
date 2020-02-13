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
	Weapon* pistol = new Weapon(RIFLE);
	m_weapons.push_back(pistol);
	m_ready = true;
	m_time = 0;

	m_material.ambient = XMVectorSet(0.5, 0.5, 0.5, -1);
	m_material.diffuse = XMVectorSet(0.0, 0.0, 0.0, -1);
	if (playerId == 0)
		m_material.emission = XMVector3Normalize(XMVectorSet(1, 0, 0, -1));
		//m_material.diffuse = XMVector3Normalize(XMVectorSet(80, 10, 180, 0));
	else if (playerId == 1)
		m_material.emission = XMVector3Normalize(XMVectorSet(0, 1, 0, -1));
		//m_material.diffuse = XMVector3Normalize(XMVectorSet(10, 189, 198, 0));
	else if (playerId == 2)
		m_material.emission = XMVector3Normalize(XMVectorSet(0, 0, 1, -1));
		//m_material.diffuse = XMVector3Normalize(XMVectorSet(255, 0, 255, 0));
	else if (playerId == 3)
		m_material.emission = XMVector3Normalize(XMVectorSet(1, 1, 0, -1));
		//m_material.diffuse = XMVector3Normalize(XMVectorSet(19, 62, 255, 0));
	// TODO add init
}

void Robot::setPlayerId(int playerId)
{
	m_playerId = playerId;
}

int Robot::getPlayerId()
{
	return m_playerId;
}

void Robot::damagePlayer(int damage)
{
	m_health -= damage;
	m_material.emission = XMVector3Normalize(m_material.emission) * (float)m_health / 100.0f;
	removeResource();
}

void Robot::setHealth(int health)
{
	m_health = health;
	if (m_playerId == 0)
		m_material.emission = XMVector3Normalize(XMVectorSet(1, 0, 0, -1));
	else if (m_playerId == 1)
		m_material.emission = XMVector3Normalize(XMVectorSet(0, 1, 0, -1));
	else if (m_playerId == 2)
		m_material.emission = XMVector3Normalize(XMVectorSet(0, 0, 1, -1));
	else if (m_playerId == 3)
		m_material.emission = XMVector3Normalize(XMVectorSet(1, 1, 0, -1));
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
	if (m_currentWeapon[RIGHT] != -1 && m_weapons[m_currentWeapon[RIGHT]]->getType() == MOVEMENT)
		return m_velocity * m_weapons[m_currentWeapon[RIGHT]]->getSpeed();
	if (m_currentWeapon[LEFT] != -1 && m_weapons[m_currentWeapon[LEFT]]->getType() == MOVEMENT)
		return m_velocity * m_weapons[m_currentWeapon[LEFT]]->getSpeed();
	return m_velocity;
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

void Robot::useWeapon(int side, float dt)
{
	if (m_currentWeapon[side] != -1 && (m_weapons[m_currentWeapon[side]]->getType() == PISTOL || m_weapons[m_currentWeapon[side]]->getType() == RIFLE))
		m_weapons[m_currentWeapon[side]]->shoot(getPosition(), m_currentRotation, side, dt);
	else if (m_currentWeapon[side] != -1 && m_weapons[m_currentWeapon[side]]->getType() == MOVEMENT)
		m_weapons[m_currentWeapon[side]]->speedUp();
	else if (m_currentWeapon[side] != -1 && m_weapons[m_currentWeapon[side]]->getType() == SHIELD)
		m_weapons[m_currentWeapon[side]]->shield();
}

void Robot::changeWeapon(int side)
{
	if (m_weapons.size() > 2) 
	{
		m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		if (m_currentWeapon[side] == m_currentWeapon[(side + 1) % 2])
			m_currentWeapon[side] = (m_currentWeapon[side] + 1) % (int)m_weapons.size();
		
		if (side == RIGHT)
			m_weapons[m_currentWeapon[RIGHT]]->setRelativePos(XMVectorSet(1.4f, 0.4f, 0.2f, 0.0f));
		else
			m_weapons[m_currentWeapon[LEFT]]->setRelativePos(XMVectorSet(-1.4f, 0.4f, 0.2f, 0.0f));
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

	for (int i = 0; i < m_weapons.size(); i++)
	{
		m_weapons[i]->updateTime(dt);
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

void Robot::release()
{
	// TODO realease resource?
	for (int i = 0; i < m_weapons.size(); i++)
	{
		delete m_weapons[i];
	}
}
